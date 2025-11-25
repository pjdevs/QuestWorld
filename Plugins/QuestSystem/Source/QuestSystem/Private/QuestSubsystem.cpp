// Copyright pjdevs. All Rights Reserved.


#include "QuestSubsystem.h"

#include "FlowAsset.h"
#include "FlowSubsystem.h"
#include "QuestLogChannels.h"
#include "SpudSubsystem.h"
#include "Assets/QuestDataAsset.h"
#include "Engine/AssetManager.h"

// Subsystem

void UQuestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	USpudSubsystem* SpudSubsystem = Collection.InitializeDependency<USpudSubsystem>();
	SpudSubsystem->AddPersistentGlobalObjectWithName(this, "QuestSubsystem");
}

bool UQuestSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return Super::ShouldCreateSubsystem(Outer);
}

void UQuestSubsystem::SpudPostRestore_Implementation(const USpudState* State)
{
	RestoreQuests(SpudQuestSaveData);
}

void UQuestSubsystem::SpudPreStore_Implementation(const USpudState* State)
{
	SpudQuestSaveData = GetQuestSave();
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
	
	QuestStatesById.Add(QuestId, FQuestState(QuestId, QuestDataAsset));

	if (UFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UFlowSubsystem>())
	{
		if (UFlowAsset* QuestFlowAsset = QuestDataAsset->QuestFlowAsset)
		{
			UFlowAsset* QuestFlowInstance = FlowSubsystem->CreateRootFlow(this, QuestFlowAsset, false);
			QuestFlowsById.Add(QuestId, QuestFlowInstance);
			QuestFlowInstance->StartFlow();
		}
	}
	
	UE_LOG(LogQuest, Verbose, TEXT("Quest started."));

	OnQuestStarted.ExecuteIfBound(QuestId);
}

void UQuestSubsystem::CompleteQuest(FQuestId QuestId)
{
	UE_LOG(LogQuest, Verbose, TEXT("Completing Quest %s."), *QuestId.ToString());

	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	const FQuestState& Quest = QuestStatesById[QuestId];
	
	if (Quest.IsCompleted())
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest already completed."));
		return;
	}

	if (UFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UFlowSubsystem>())
	{
		if (UFlowAsset* QuestFlowAsset = Quest.GetQuestAsset()->QuestFlowAsset)
		{
			FlowSubsystem->FinishRootFlow(this, QuestFlowAsset, EFlowFinishPolicy::Abort);
			QuestFlowsById.Remove(QuestId);
		}
	}

	UE_LOG(LogQuest, Verbose, TEXT("Quest %s completed."), *QuestId.ToString());

	OnQuestCompleted.ExecuteIfBound(QuestId);
}

void UQuestSubsystem::StartObjective(FQuestId QuestId, const FGameplayTag& ObjectiveId)
{
	UE_LOG(LogQuest, Verbose, TEXT("Starting objective %s."), *ObjectiveId.ToString());
	
	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	FQuestState& Quest = QuestStatesById[QuestId];
	Quest.StartObjective(ObjectiveId, GetWorld());

	UE_LOG(LogQuest, Verbose, TEXT("Objective started."));

	OnQuestUpdated.ExecuteIfBound(QuestId);
}

void UQuestSubsystem::CompleteObjective(FQuestId QuestId, const FGameplayTag& ObjectiveId)
{
	UE_LOG(LogQuest, Verbose, TEXT("Completing objective %s."), *ObjectiveId.ToString());
	
	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	FQuestState& Quest = QuestStatesById[QuestId];
	Quest.CompleteObjective(ObjectiveId);

	UE_LOG(LogQuest, Verbose, TEXT("Objective completed."));

	OnQuestUpdated.ExecuteIfBound(QuestId);
}

void UQuestSubsystem::ProgressObjective(FQuestId QuestId, const FGameplayTag& ObjectiveId, int Progress)
{
	if (!QuestStatesById.Contains(QuestId))
	{
		UE_LOG(LogQuest, Verbose, TEXT("Quest not even started."));
		return;
	}

	FQuestState& Quest = QuestStatesById[QuestId];
	Quest.ProgressObjective(ObjectiveId, Progress);

	OnQuestUpdated.ExecuteIfBound(QuestId);
}

void UQuestSubsystem::SubmitQuestEvent(UBaseQuestEvent* Event)
{
	TArray<FQuestId> QuestToCompleteIds;
	
	for (auto& [QuestId, Quest] : QuestStatesById)
	{
		if (Quest.IsCompleted())
		{
			continue;
		}
		
		if (Quest.OnQuestEvent(GetWorld(), Event))
		{
			OnQuestUpdated.ExecuteIfBound(QuestId);
		}
		
		if (Quest.IsCompleted())
		{
			QuestToCompleteIds.Add(QuestId);
		}
	}

	for (const FQuestId& QuestId : QuestToCompleteIds)
	{
		CompleteQuest(QuestId);
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
			ObjectiveState.GetDescription(),
			ObjectiveState.GetCurrentProgress(),
			ObjectiveState.GetTargetProgress(),
			ObjectiveState.IsCompleted()
		});
	}

	return FQuestDescription
	{
		QuestId,
		QuestDataAsset->Title,
		QuestDataAsset->Description,
		QuestObjectiveDescriptions,
		QuestState->IsCompleted()
	};
}

void UQuestSubsystem::RestoreQuests(const FQuestSaveData& QuestSave)
{
	QuestStatesById.Empty();

	for (const FQuestStateSaveData& QuestData : QuestSave.QuestStates)
	{
		const FQuestId& QuestId = QuestData.QuestId;
		const UQuestDataAsset* QuestAsset = GetQuestAsset(QuestId);
		FQuestState QuestState(QuestId, QuestAsset);

		for (const FQuestObjectiveSateSaveData& ObjectiveData : QuestData.ObjectiveStates)
		{
			QuestState.StartObjective(ObjectiveData.ObjectiveId, GetWorld());
			QuestState.ProgressObjective(ObjectiveData.ObjectiveId, ObjectiveData.CurrentProgress);

			if (ObjectiveData.bIsCompleted)
			{
				QuestState.CompleteObjective(ObjectiveData.ObjectiveId);
			}
		}

		if (QuestData.bIsCompleted)
		{
			QuestState.CompleteQuest();
		}
		
		QuestStatesById.Add(
			QuestId,
			QuestState
		);

		UFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UFlowSubsystem>();
		UFlowAsset* QuestFlowAsset = QuestAsset->QuestFlowAsset;
		
		if (!QuestData.bIsCompleted && FlowSubsystem && QuestFlowAsset)
		{
			UFlowAsset* QuestFlowInstance = FlowSubsystem->CreateRootFlow(this, QuestFlowAsset, false);
			QuestFlowInstance->LoadInstance(QuestData.QuestFlowSave);
			QuestFlowsById.Add(QuestId, QuestFlowInstance);
		}
	}
}

FQuestSaveData UQuestSubsystem::GetQuestSave() const
{
	FQuestSaveData QuestSaveData;
	QuestSaveData.QuestStates = TArray<FQuestStateSaveData>();

	for (const auto& [QuestId, ActiveQuest] : QuestStatesById)
	{
		FQuestStateSaveData QuestStateData
		{
			.QuestId = QuestId,
			.ObjectiveStates = TArray<FQuestObjectiveSateSaveData>()
		};

		for (auto& [ObjectiveId, Objective] : ActiveQuest.GetObjectives())
		{
			FQuestObjectiveSateSaveData ObjectiveStateData
			{
				.ObjectiveId = Objective.GetObjectiveId(), 
				.CurrentProgress = Objective.GetCurrentProgress(),
				.bIsCompleted = Objective.IsCompleted(),
			};
			QuestStateData.ObjectiveStates.Add(ObjectiveStateData);
		}

		if (const TObjectPtr<UFlowAsset>* QuestFlowInstancePtr = QuestFlowsById.Find(QuestId))
		{
			TArray<FFlowAssetSaveData> Records;
			Records.Reserve(1);
			QuestFlowInstancePtr->Get()->SaveInstance(Records);
			QuestStateData.QuestFlowSave = Records[0];
		}

		QuestSaveData.QuestStates.Add(QuestStateData);
	}

	return QuestSaveData;
}

FQuestId UQuestSubsystem::GetQuestIdFromFlow(UFlowAsset* QuestFlowInstance) const
{
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
}