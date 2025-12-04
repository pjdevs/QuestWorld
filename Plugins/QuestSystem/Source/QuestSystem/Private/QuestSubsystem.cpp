// Copyright pjdevs. All Rights Reserved.


#include "QuestSubsystem.h"

#include "FlowAsset.h"
#include "FlowSubsystem.h"
#include "QuestLogChannels.h"
#include "SpudSubsystem.h"
#include "Assets/QuestDataAsset.h"
#include "Engine/AssetManager.h"
#include "Flow/QuestFlowAsset.h"
#include "Flow/QuestFlowNode_QuestObjective.h"
#include "Flow/QuestFlowNode_SubGraph.h"
#include "Kismet/GameplayStatics.h"


// Subsystem

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	USpudSubsystem* SpudSubsystem = Collection.InitializeDependency<USpudSubsystem>();
	SpudSubsystem->AddPersistentGlobalObjectWithName(this, "QuestSubsystem");
	SpudSubsystem->PostLoadGame.AddDynamic(this, &UQuestSubsystem::OnSpudPostLoadGame);
}

void UQuestSubsystem::SpudPostRestore_Implementation(const USpudState* State)
{
	LoadQuestSave(SpudQuestSaveData);
}

void UQuestSubsystem::SpudPreStore_Implementation(const USpudState* State)
{
	SpudQuestSaveData = MakeQuestSave();
}

// Quest

void UQuestSubsystem::StartQuest(FQuestId QuestId)
{
	UE_LOG(LogQuest, Verbose, TEXT("Starting Quest %s."), *QuestId.ToString());

	if (QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest already started or completed."));
		return;
	}

	UQuestDataAsset* QuestDataAsset = GetQuestAsset(QuestId);

	if (QuestDataAsset == nullptr)
	{
		UE_LOG(LogQuest, Error, TEXT("Quest asset not found."));
		return;
	}
	
	FQuestState& QuestState = QuestStatesById.Add(QuestId, FQuestState(QuestId, QuestDataAsset));
	StartListeningQuestEvents(QuestState);

	if (UFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UFlowSubsystem>())
	{
		if (UQuestFlowAsset* QuestFlowAsset = QuestDataAsset->QuestFlowAsset.LoadSynchronous())
		{
			UQuestFlowAsset* QuestFlowInstance = Cast<UQuestFlowAsset>(
				FlowSubsystem->CreateRootFlow(this, QuestFlowAsset, false)
			);
			QuestFlowsById.Add(QuestId, QuestFlowInstance);
			QuestFlowInstance->StartFlow();
		}
	}
	
	UE_LOG(LogQuest, Verbose, TEXT("Quest started."));

	OnQuestStarted.Broadcast(QuestId);
}

void UQuestSubsystem::SucceedQuest(FQuestId QuestId)
{
	CompleteQuest(QuestId, EQuestCompletionState::Succeeded);
}

void UQuestSubsystem::FailQuest(const FQuestId& QuestId)
{
	CompleteQuest(QuestId, EQuestCompletionState::Failed);
}

void UQuestSubsystem::StartQuestPhase(FQuestId QuestId, const FName& Phase)
{
	UE_LOG(LogQuest, Verbose, TEXT("Starting phase %s for quest %s."), *Phase.ToString(), *QuestId.ToString());
	
	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	FQuestState& Quest = QuestStatesById[QuestId];
	Quest.StartPhase(Phase);

	OnQuestUpdated.Broadcast(QuestId);
}

void UQuestSubsystem::CompleteQuestActivePhase(FQuestId QuestId)
{
	UE_LOG(LogQuest, Verbose, TEXT("Completing active phase for quest %s."), *QuestId.ToString());
	
	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	FQuestState& Quest = QuestStatesById[QuestId];
	Quest.CompleteActivePhase();

	OnQuestUpdated.Broadcast(QuestId);
}

void UQuestSubsystem::StartObjective(FQuestId QuestId, const FName& ObjectiveId)
{
	UE_LOG(LogQuest, Verbose, TEXT("Starting objective %s for quest %s."), *ObjectiveId.ToString(), *QuestId.ToString());
	
	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	FQuestState& Quest = QuestStatesById[QuestId];
	Quest.StartObjective(ObjectiveId, GetWorld());

	UE_LOG(LogQuest, Verbose, TEXT("Objective started."));

	OnQuestUpdated.Broadcast(QuestId);
}

void UQuestSubsystem::SucceedObjective(FQuestId QuestId, const FName& ObjectiveId)
{
	CompleteObjective(QuestId, ObjectiveId, EQuestObjectiveCompletionState::Succeeded);
}

void UQuestSubsystem::FailObjective(FQuestId QuestId, const FName& ObjectiveId)
{
	CompleteObjective(QuestId, ObjectiveId, EQuestObjectiveCompletionState::Failed);
}

void UQuestSubsystem::ProgressObjective(FQuestId QuestId, const FName& ObjectiveId, int Progress)
{
	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	FQuestState& Quest = QuestStatesById[QuestId];
	Quest.ProgressObjective(ObjectiveId, Progress);

	OnQuestUpdated.Broadcast(QuestId);
}

void UQuestSubsystem::SubmitQuestEvent(UBaseQuestEvent* Event)
{
	// Handle auto complete
	TArray<FQuestId> QuestToCompleteIds;
	
	for (auto& [QuestId, Quest] : QuestStatesById)
	{
		if (Quest.IsCompleted())
		{
			continue;
		}
		
		if (Quest.OnQuestEvent(GetWorld(), Event))
		{
			OnQuestUpdated.Broadcast(QuestId);
		}
		
		if (Quest.IsCompleted())
		{
			QuestToCompleteIds.Add(QuestId);
		}
	}

	// Complete quest
	for (const FQuestId& QuestId : QuestToCompleteIds)
	{
		HandleQuestCompleted(QuestStatesById[QuestId]);
	}
}

TArray<FQuestId> UQuestSubsystem::GetKnownQuests() const
{
	TArray<FQuestId> KnownQuests;
	KnownQuests.Reserve(QuestStatesById.Num());
	QuestStatesById.GenerateKeyArray(KnownQuests);

	return KnownQuests;
}

bool UQuestSubsystem::IsQuestStarted(const FQuestId& QuestId) const
{
	if (const FQuestState* QuestState = QuestStatesById.Find(QuestId))
	{
		return !QuestState->IsCompleted();
	}

	return false;
}

bool UQuestSubsystem::IsQuestCompleted(const FQuestId& QuestId) const
{
	if (const FQuestState* QuestState = QuestStatesById.Find(QuestId))
	{
		return QuestState->IsCompleted();
	}

	return false;
}

bool UQuestSubsystem::IsQuestSucceeded(const FQuestId& QuestId) const
{
	if (const FQuestState* QuestState = QuestStatesById.Find(QuestId))
	{
		return QuestState->GetCompletionState() == EQuestCompletionState::Succeeded;
	}

	return false;
}

bool UQuestSubsystem::IsQuestFailed(const FQuestId& QuestId) const
{
	if (const FQuestState* QuestState = QuestStatesById.Find(QuestId))
	{
		return QuestState->GetCompletionState() == EQuestCompletionState::Failed;
	}

	return false;
}

bool UQuestSubsystem::IsObjectiveCompleted(const FQuestId& QuestId, const FName& ObjectiveId) const
{
	if (const FQuestState* QuestState = QuestStatesById.Find(QuestId))
	{
		return QuestState->IsObjectiveCompleted(ObjectiveId);
	}

	return false;
}

bool UQuestSubsystem::IsObjectiveSucceeded(const FQuestId& QuestId, const FName& ObjectiveId) const
{
	if (const FQuestState* QuestState = QuestStatesById.Find(QuestId))
	{
		return QuestState->IsObjectiveSucceeded(ObjectiveId);
	}

	return false;
}

bool UQuestSubsystem::IsObjectiveFailed(const FQuestId& QuestId, const FName& ObjectiveId) const
{
	if (const FQuestState* QuestState = QuestStatesById.Find(QuestId))
	{
		return QuestState->IsObjectiveFailed(ObjectiveId);
	}

	return false;
}

FQuestDescription UQuestSubsystem::GetQuestDescription(const FQuestId& QuestId)
{
	const FQuestState* QuestState = QuestStatesById.Find(QuestId);
	
	if (QuestState == nullptr)
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest %s not started or completed."), *QuestId.ToString());
		return FQuestDescription{};
	}
	
	const UQuestDataAsset* QuestDataAsset = GetQuestAsset(QuestId);

	if (QuestDataAsset == nullptr)
	{
		UE_LOG(LogQuest, Error, TEXT("Cannot get description of quest %s: QuestDataAsset not found."), *QuestId.ToString());
		return FQuestDescription{};
	}
	
	TArray<FQuestObjectiveDescription> QuestObjectiveDescriptions;

	for (const auto& [ObjectiveId, ObjectiveState] : QuestState->GetObjectives())
	{
		QuestObjectiveDescriptions.Add(FQuestObjectiveDescription
		{
			 .Description = ObjectiveState.GetDescription(),
			.CurrentValue = ObjectiveState.GetCurrentProgress(),
			.TargetValue = ObjectiveState.GetTargetProgress(),
			.CompletionState = ObjectiveState.GetCompletionState(),
			.bIsOptional = ObjectiveState.IsOptional(),
			.bIsActiveInPhase = ObjectiveState.IsActiveInPhase(QuestState->GetActivePhase())
		});
	}

	return FQuestDescription
	{
		.QuestId = QuestId,
		.QuestType = QuestDataAsset->QuestType,
		.Title = QuestDataAsset->Title,
		.Description = QuestDataAsset->Description,
		.Objectives = QuestObjectiveDescriptions,
		.CompletionState = QuestState->GetCompletionState()
	};
}

void UQuestSubsystem::LoadQuestSave(const FQuestSaveData& QuestSave)
{
	UnloadAll();
	LoadedQuestSaveData = QuestSave;
	bIsLoadedSavePendingRestore = true;
}

// TODO Unify with start quest

void UQuestSubsystem::RestoreQuestsFromSave()
{
	if (!bIsLoadedSavePendingRestore)
	{
		return;
	}
	
	for (const FQuestStateSaveData& QuestData : LoadedQuestSaveData.QuestStates)
	{
		const FQuestId& QuestId = QuestData.QuestId;
		const UQuestDataAsset* QuestAsset = GetQuestAsset(QuestId);
		FQuestState QuestState(QuestId, QuestAsset);

		for (const FQuestObjectiveSateSaveData& ObjectiveData : QuestData.ObjectiveStates)
		{
			QuestState.StartObjective(ObjectiveData.ObjectiveId, GetWorld());
			QuestState.ProgressObjective(ObjectiveData.ObjectiveId, ObjectiveData.CurrentProgress);

			if (ObjectiveData.CompletionState != EQuestObjectiveCompletionState::Started)
			{
				QuestState.CompleteObjective(ObjectiveData.ObjectiveId, ObjectiveData.CompletionState);
			}
		}

		QuestState.RestoreCompletedPhases(QuestData.CompletedPhases);

		if (QuestData.ActivePhase != NAME_None)
		{
			QuestState.StartPhase(QuestData.ActivePhase);
		}

		if (QuestData.CompletionState != EQuestCompletionState::Started)
		{
			QuestState.SetCompletionState(QuestData.CompletionState);
		}
		else
		{
			StartListeningQuestEvents(QuestState);
		}
		
		QuestStatesById.Add(
			QuestId,
			QuestState
		);

		UFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UFlowSubsystem>();
		UQuestFlowAsset* QuestFlowAsset = QuestAsset->QuestFlowAsset.LoadSynchronous();
		
		if (!QuestState.IsCompleted())
		{
			if (FlowSubsystem && QuestFlowAsset)
			{
				// Trick to make subgraph saved data available
				// TODO add subgraphs saves in parent quest graph and override sub quest sub graph node onload function
				// to load the node from the parent graph instead of the flow subsystem 
				UFlowSaveGame* FlowSaveGame = FlowSubsystem->GetLoadedSaveGame();

				if (FlowSaveGame == nullptr)
				{
					FlowSaveGame =  Cast<UFlowSaveGame>(
						UGameplayStatics::CreateSaveGameObject(UFlowSaveGame::StaticClass())
					);
				}

				for (FFlowAssetSaveData FlowAssetSaveData : QuestData.QuestFlowInstancesSave)
				{
					FlowSaveGame->FlowInstances.Add(FlowAssetSaveData);
				}

				FlowSubsystem->OnGameLoaded(FlowSaveGame);
				// end trick
				
				UQuestFlowAsset* QuestFlowInstance = Cast<UQuestFlowAsset>(
					FlowSubsystem->CreateRootFlow(this, QuestFlowAsset, false)
				);
				QuestFlowsById.Add(QuestId, QuestFlowInstance);

				const FFlowAssetSaveData* FlowInstanceSaveData = QuestData.QuestFlowInstancesSave.FindByPredicate(
					[QuestFlowInstance](const FFlowAssetSaveData& SaveData)
					{
						return SaveData.InstanceName == QuestFlowInstance->GetFName().ToString();
					}
				);

				if (FlowInstanceSaveData)
				{
					QuestFlowInstance->LoadInstance(*FlowInstanceSaveData);
				}
			}

			OnQuestUpdated.Broadcast(QuestId);
		}
	}

	bIsLoadedSavePendingRestore = false;
}

FQuestSaveData UQuestSubsystem::MakeQuestSave() const
{
	FQuestSaveData QuestSaveData;
	QuestSaveData.QuestStates = TArray<FQuestStateSaveData>();

	for (const auto& [QuestId, Quest] : QuestStatesById)
	{
		FQuestStateSaveData QuestStateData
		{
			.QuestId = QuestId,
			.ObjectiveStates = TArray<FQuestObjectiveSateSaveData>(),
			.CompletionState = Quest.GetCompletionState(),
			.ActivePhase = Quest.GetActivePhase(),
			.CompletedPhases = Quest.GetCompletedPhases()
		};

		for (auto& [ObjectiveId, Objective] : Quest.GetObjectives())
		{
			FQuestObjectiveSateSaveData ObjectiveStateData
			{
				.ObjectiveId = Objective.GetObjectiveId(), 
				.CurrentProgress = Objective.GetCurrentProgress(),
				.CompletionState = Objective.GetCompletionState(),
			};
			QuestStateData.ObjectiveStates.Add(ObjectiveStateData);
		}

		if (const TObjectPtr<UQuestFlowAsset>* QuestFlowInstancePtr = QuestFlowsById.Find(QuestId))
		{
			QuestFlowInstancePtr->Get()->SaveInstance(QuestStateData.QuestFlowInstancesSave);
		}

		QuestSaveData.QuestStates.Add(QuestStateData);
	}

	return QuestSaveData;
}

FQuestId UQuestSubsystem::GetQuestIdFromFlow(UQuestFlowAsset* QuestFlowInstance) const
{
	if (!QuestFlowInstance)
	{
		return FQuestId();
	}
	
	if (
		const UQuestFlowNode_SubGraph* QuestSubFlow = Cast<UQuestFlowNode_SubGraph>(
			QuestFlowInstance->GetNodeOwningThisAssetInstance()
		)
	)
	{
		return GetQuestIdFromFlow(Cast<UQuestFlowAsset>(QuestSubFlow->GetFlowAsset()));	
	}
	
	const FQuestId* QuestIdPtr = QuestFlowsById.FindKey(QuestFlowInstance);
	return QuestIdPtr != nullptr ? *QuestIdPtr : FQuestId();
}

UQuestDataAsset* UQuestSubsystem::GetQuestAsset(const FQuestId& QuestId)
{
	if (QuestAssetsById.Contains(QuestId))
	{
		return QuestAssetsById[QuestId];
	}

	const TSharedPtr<FStreamableHandle> Handle = UAssetManager::Get().LoadPrimaryAsset(QuestId.QuestAssetId);
	Handle->WaitUntilComplete();

	UQuestDataAsset* QuestAsset = Handle->GetLoadedAsset<UQuestDataAsset>();

	if (QuestAsset)
	{
		QuestAssetsById.Add(QuestId, QuestAsset);
	}

	return QuestAsset;
}

void UQuestSubsystem::UnloadAll()
{
	UAssetManager& AssetManager = UAssetManager::Get();
	
	for (auto& [QuestId, QuestAsset] : QuestAssetsById)
	{
		AssetManager.UnloadPrimaryAsset(QuestId.QuestAssetId);
	}

	if (UFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UFlowSubsystem>())
	{
		FlowSubsystem->FinishAllRootFlows(this, EFlowFinishPolicy::Abort);
	}

	QuestAssetsById.Empty();
	QuestAssetsById.Empty();
	QuestFlowsById.Empty();
}

void UQuestSubsystem::CompleteQuest(const FQuestId& QuestId, EQuestCompletionState CompletionState)
{
	ensureAlways(CompletionState != EQuestCompletionState::Started);
	
	UE_LOG(LogQuest, Verbose, TEXT("Completing Quest %s. State: %d."), *QuestId.ToString(), CompletionState);

	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	FQuestState& QuestState = QuestStatesById[QuestId];
	
	if (QuestState.IsCompleted())
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest already completed."));
		return;
	}
	
	QuestState.SetCompletionState(CompletionState);
	HandleQuestCompleted(QuestState);

	UE_LOG(LogQuest, Verbose, TEXT("Quest %s completed."), *QuestId.ToString());
}

void UQuestSubsystem::CompleteObjective(
	const FQuestId& QuestId,
	const FName& ObjectiveId,
	EQuestObjectiveCompletionState CompletionState
)
{
	UE_LOG(LogQuest, Verbose, TEXT("Completing objective %s."), *ObjectiveId.ToString());
	
	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	FQuestState& Quest = QuestStatesById[QuestId];
	Quest.CompleteObjective(ObjectiveId, CompletionState);

	UE_LOG(LogQuest, Verbose, TEXT("Objective completed."));

	OnQuestUpdated.Broadcast(QuestId);
}

void UQuestSubsystem::HandleQuestCompleted(FQuestState& QuestState)
{
	const FQuestId& QuestId = QuestState.GetQuestId();
	
	StopListeningQuestEvents(QuestState);
	
	if (UFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UFlowSubsystem>())
	{
		if (QuestState.GetQuestAsset()->QuestFlowAsset.ToSoftObjectPath().IsValid())
		{
			UQuestFlowAsset* QuestFlowAsset = QuestFlowsById[QuestId];
			FlowSubsystem->FinishRootFlow(this, QuestFlowAsset, EFlowFinishPolicy::Abort);
			QuestFlowsById.Remove(QuestId);
		}
	}

	UE_LOG(LogQuest, Verbose, TEXT("Quest %s completed."), *QuestId.ToString());

	OnQuestCompleted.Broadcast(QuestId);
}

void UQuestSubsystem::StartListeningQuestEvents(FQuestState& QuestState) const
{
	const FQuestId& QuestId = QuestState.GetQuestId();
	QuestState.OnObjectiveCompleted.BindLambda(
		[this, QuestId](const FName& ObjectiveId, EQuestObjectiveCompletionState CompletionState)
		{
			OnObjectiveCompleted.Broadcast(QuestId, ObjectiveId, CompletionState);
		}
	);
}

void UQuestSubsystem::StopListeningQuestEvents(FQuestState& QuestState) const
{
	QuestState.OnObjectiveCompleted.Unbind();
}

void UQuestSubsystem::OnSpudPostLoadGame(const FString& SlotName, bool bSuccess)
{
	if (bSuccess)
	{
		RestoreQuestsFromSave();
	}
}
