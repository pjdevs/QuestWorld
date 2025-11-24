// Copyright pjdevs. All Rights Reserved.


#include "QuestSubsystem.h"

#include "FlowAsset.h"
#include "FlowSubsystem.h"
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
	UE_LOG(LogTemp, Display, TEXT("Starting Quest %s."), *QuestId.ToString());

	UFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UFlowSubsystem>();

	if (!FlowSubsystem)
	{
		UE_LOG(LogTemp, Error, TEXT("No FlowSubsystem. Could not StartQuest %s."), *QuestId.ToString());
		return;
	}

	if (CompletedQuestIds.Contains(QuestId))
	{
		UE_LOG(LogTemp, Display, TEXT("Quest already completed."));
		return;
	}
	
	if (ActiveQuestsById.Contains(QuestId))
	{
		UE_LOG(LogTemp, Display, TEXT("Quest already started."));
		return;
	}

	UQuestDataAsset* QuestDataAsset = GetQuestAsset(QuestId);

	if (QuestDataAsset == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Quest not found."));
		return;
	}
	
	const FActiveQuest ActiveQuest(QuestDataAsset);
	ActiveQuestsById.Add(QuestId, ActiveQuest);
	
	OnQuestStarted.ExecuteIfBound(QuestId);

	if (UFlowAsset* QuestFlowAsset = QuestDataAsset->QuestFlowAsset)
	{
		UFlowAsset* QuestFlowInstance = FlowSubsystem->CreateRootFlow(this, QuestFlowAsset, false);
		QuestFlowInstance->StartFlow();
		ActiveQuestFlowsById.Add(QuestId, QuestFlowInstance);
	}

	UE_LOG(LogTemp, Display, TEXT("Quest started."));
}

void UQuestSubsystem::CompleteQuest(FQuestId QuestId)
{
	const UQuestDataAsset* QuestDataAsset = GetQuestAsset(QuestId);
	
	if (!QuestDataAsset || CompletedQuestIds.Contains(QuestId))
	{
		return;
	}

	if (ActiveQuestsById.Contains(QuestId))
	{
		ActiveQuestsById.Remove(QuestId);
	}
	
	CompletedQuestIds.Add(QuestId);

	UE_LOG(LogTemp, Display, TEXT("Quest %s completed."), *QuestId.ToString());

	OnQuestCompleted.ExecuteIfBound(QuestId);
}

TArray<FQuestId> UQuestSubsystem::GetActiveQuests() const
{
	TArray<FQuestId> QuestIds;

	for (auto& Quest : ActiveQuestsById)
	{
		QuestIds.Add(Quest.Key);
	}
	
	return QuestIds;
}

TArray<FQuestId> UQuestSubsystem::GetCompletedQuests() const
{
	return CompletedQuestIds;
}

void UQuestSubsystem::SubmitQuestEvent(UBaseQuestEvent* Event)
{
	TArray<FQuestId> QuestToCompleteIds;
	
	for (auto& Tuple : ActiveQuestsById)
	{
		const FQuestId& ActiveQuestId = Tuple.Key;
		FActiveQuest& ActiveQuest = Tuple.Value;
		
		if (ActiveQuest.OnQuestEvent(GetWorld(), Event))
		{
			OnQuestUpdated.ExecuteIfBound(ActiveQuestId);
		}
		
		if (ActiveQuest.IsCompleted())
		{
			QuestToCompleteIds.Add(Tuple.Key);
		}
	}

	for (auto& QuestId : QuestToCompleteIds)
	{
		CompleteQuest(QuestId);
	}
}

FQuestDescription UQuestSubsystem::GetQuestDescription(const FQuestId& QuestId)
{
	const UQuestDataAsset* QuestDataAsset = GetQuestAsset(QuestId);

	if (QuestDataAsset == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Cannot get description of quest %s: QuestDataAsset not found."), *QuestId.ToString());
		return FQuestDescription{};
	}

	const bool bIsQuestActive = ActiveQuestsById.Contains(QuestId);
	const bool bIsQuestCompleted = CompletedQuestIds.Contains(QuestId);
	
	TArray<FQuestObjectiveDescription> QuestObjectiveDescriptions;

	for (const TObjectPtr<UQuestObjective>& ObjectiveAsset : QuestDataAsset->Objectives)
	{
		int CurrentProgress = 0;
		bool bIsObjectiveCompleted = false;

		if (bIsQuestActive)
		{
			const FActiveQuestObjective* ActiveObjective = ActiveQuestsById
				.FindChecked(QuestId)
				.GetActiveObjective(ObjectiveAsset->ObjectiveId);
			
			CurrentProgress = ActiveObjective->GetCurrentProgress();
			bIsObjectiveCompleted = ActiveObjective->IsObjectiveCompleted();
		}
		else if (bIsQuestCompleted)
		{
			CurrentProgress = ObjectiveAsset->GetTargetValue();
			bIsObjectiveCompleted = true;
		}
		// not started or completed, not found quest
		else 
		{
			// show any objective??
		}

		QuestObjectiveDescriptions.Add(FQuestObjectiveDescription
		{
			ObjectiveAsset->ObjectiveDescription,
			CurrentProgress,
			ObjectiveAsset->GetTargetValue(),
			bIsObjectiveCompleted
		});
	}

	return FQuestDescription
	{
		QuestId,
		QuestDataAsset->Title,
		QuestDataAsset->Description,
		QuestObjectiveDescriptions,
		!bIsQuestActive
	};
}

void UQuestSubsystem::RestoreQuests(const FQuestSaveData& QuestSave)
{
	CompletedQuestIds.Empty();
	CompletedQuestIds = QuestSave.CompletedQuests;
	
	ActiveQuestsById.Empty();

	for (const FActiveQuestSaveData& QuestData : QuestSave.ActiveQuests)
	{
		const FQuestId& QuestId = QuestData.QuestId;
		UQuestDataAsset* QuestAsset = GetQuestAsset(QuestId);
		FActiveQuest ActiveQuest(QuestAsset);

		for (int i = 0; i < QuestData.ActiveObjectives.Num(); ++i)
		{
			const FActiveQuestObjectiveSaveData& ObjectiveData = QuestData.ActiveObjectives[i];
			ActiveQuest.StartObjective(ObjectiveData.ObjectiveId, GetWorld());
			ActiveQuest.ProgressObjective(ObjectiveData.ObjectiveId, ObjectiveData.CurrentProgress);
		}
		
		ActiveQuestsById.Add(
			QuestId,
			ActiveQuest
		);

		UFlowSubsystem* FlowSubsystem = GetGameInstance()->GetSubsystem<UFlowSubsystem>();
		UFlowAsset* QuestFlowAsset = QuestAsset->QuestFlowAsset;
		
		if (FlowSubsystem && QuestFlowAsset)
		{
			UFlowAsset* QuestFlowInstance = FlowSubsystem->CreateRootFlow(this, QuestFlowAsset, false);
			QuestFlowInstance->LoadInstance(QuestData.QuestFlowSave);
			ActiveQuestFlowsById.Add(QuestId, QuestFlowInstance);
		}
	}
}

FQuestSaveData UQuestSubsystem::GetQuestSave() const
{
	FQuestSaveData QuestSaveData;
	QuestSaveData.ActiveQuests = TArray<FActiveQuestSaveData>();
	QuestSaveData.CompletedQuests = TArray<FQuestId>();

	for (const auto& [QuestId, ActiveQuest] : ActiveQuestsById)
	{
		FActiveQuestSaveData ActiveQuestData
		{
			.QuestId = QuestId,
			.ActiveObjectives = TArray<FActiveQuestObjectiveSaveData>()
		};

		for (auto It = ActiveQuest.GetActiveObjectives().CreateConstIterator(); It; ++It)
		{
			FActiveQuestObjectiveSaveData ActiveObjectiveData
			{
				.CurrentProgress = It.Value().GetCurrentProgress()
			};
			ActiveQuestData.ActiveObjectives.Add(ActiveObjectiveData);
		}

		if (const TObjectPtr<UFlowAsset>* QuestFlowInstancePtr = ActiveQuestFlowsById.Find(QuestId))
		{
			TArray<FFlowAssetSaveData> Records;
			Records.Reserve(1);
			(*QuestFlowInstancePtr)->SaveInstance(Records);

			
		}

		QuestSaveData.ActiveQuests.Add(ActiveQuestData);
	}
	
	for (const FQuestId& CompletedQuestId : CompletedQuestIds)
	{
		QuestSaveData.CompletedQuests.Add(CompletedQuestId);
	}

	return QuestSaveData;
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