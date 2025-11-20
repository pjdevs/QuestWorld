// Copyright pjdevs. All Rights Reserved.

#include "QuestService.h"
#include "ActiveQuest.h"
#include "PrimaryAssetTypes.h"
#include "QuestDescription.h"
#include "QuestSaveGame.h"
#include "Assets/QuestDataAsset.h"
#include "Engine/AssetManager.h"


UQuestServiceImpl::UQuestServiceImpl() : QuestAssetType(PrimaryAssetTypes::Quest) { }

void UQuestServiceImpl::LoadQuests(FQuestLoadedDelegate CompletionDelegate)
{
	UE_LOG(LogTemp, Display, TEXT("Loading Quests..."));

	UAssetManager& AssetManager = UAssetManager::Get();

	AssetManager.LoadPrimaryAssetsWithType(
		QuestAssetType,
		{},
		FStreamableDelegate::CreateLambda([this, CompletionDelegate]
		{
			OnQuestsLoaded(CompletionDelegate);
		})
	);
}

void UQuestServiceImpl::StartQuest(const FPrimaryAssetId& QuestId, UWorld* World)
{
	UE_LOG(LogTemp, Display, TEXT("Starting Quest %s."), *QuestId.ToString());

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

	const TObjectPtr<UQuestDataAsset>* QuestDataAssetPtr = QuestAssetsById.Find(QuestId);

	if (QuestDataAssetPtr == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Quest not found."));
		return;
	}
	
	const FActiveQuest ActiveQuest(QuestId, *QuestDataAssetPtr);
	ActiveQuestsById.Add(QuestId, ActiveQuest);
	
	bool _ = QuestStartedDelegate.ExecuteIfBound(GetQuestDescription(QuestId));
	
	if (ActiveQuest.IsCompleted())
	{
		CompleteQuest(QuestId);
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Quest started."));
}

TArray<FPrimaryAssetId> UQuestServiceImpl::GetActiveQuests() const
{
	TArray<FPrimaryAssetId> QuestIds;

	for (auto& Quest : ActiveQuestsById)
	{
		QuestIds.Add(Quest.Key);
	}
	
	return QuestIds;
}

TArray<FPrimaryAssetId> UQuestServiceImpl::GetCompletedQuests() const
{
	return CompletedQuestIds;
}

void UQuestServiceImpl::SubmitQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	TArray<FPrimaryAssetId> QuestToCompleteIds;
	
	for (auto& Tuple : ActiveQuestsById)
	{
		const FPrimaryAssetId& ActiveQuestId = Tuple.Key;
		FActiveQuest& ActiveQuest = Tuple.Value;
		
		if (ActiveQuest.OnQuestEvent(World, Event))
		{
			bool _ = QuestUpdatedDelegate.ExecuteIfBound(GetQuestDescription(ActiveQuestId));
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

TArray<FQuestDescription> UQuestServiceImpl::GetActiveQuestDescriptions() const
{
	TArray<FQuestDescription> QuestsDescriptions;

	for (auto& IdAndQuest : ActiveQuestsById)
	{
		QuestsDescriptions.Add(GetQuestDescription(IdAndQuest.Key));
	}
	
	return QuestsDescriptions;
}

TArray<FQuestDescription> UQuestServiceImpl::GetCompletedQuestDescriptions() const
{
	TArray<FQuestDescription> QuestsDescriptions;

	for (const FPrimaryAssetId& QuestId : CompletedQuestIds)
	{
		QuestsDescriptions.Add(GetQuestDescription(QuestId));
	}
	
	return QuestsDescriptions;
}

FQuestDescription UQuestServiceImpl::GetQuestDescription(const FPrimaryAssetId& QuestId) const
{
	const TObjectPtr<UQuestDataAsset>* QuestDataAssetPtr = QuestAssetsById.Find(QuestId);

	if (QuestDataAssetPtr == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Cannot get description of quest %s: QuestDataAsset not found."), *QuestId.ToString());
		return FQuestDescription{};
	}

	const UQuestDataAsset* QuestDataAsset = *QuestDataAssetPtr;
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

bool UQuestServiceImpl::IsQuestCompleted(const FPrimaryAssetId& QuestId) const
{
	return CompletedQuestIds.Contains(QuestId);
}

bool UQuestServiceImpl::IsQuestActive(const FPrimaryAssetId& QuestId) const
{
	return ActiveQuestsById.Contains(QuestId);
}

void UQuestServiceImpl::SetQuestStartedDelegate(const FQuestEventDelegate& QuestEventDelegate)
{
	QuestStartedDelegate = QuestEventDelegate;
}

void UQuestServiceImpl::SetQuestCompletedDelegate(const FQuestEventDelegate& QuestEventDelegate)
{
	QuestCompletedDelegate = QuestEventDelegate;
}

void UQuestServiceImpl::SetQuestUpdatedDelegate(const FQuestEventDelegate& QuestEventDelegate)
{
	QuestUpdatedDelegate = QuestEventDelegate;
}

void UQuestServiceImpl::RestoreQuests(const FQuestSaveData& QuestSave, UWorld* World)
{
	CompletedQuestIds.Empty();
	CompletedQuestIds = QuestSave.CompletedQuests;
	
	ActiveQuestsById.Empty();

	for (const FActiveQuestSaveData& QuestData : QuestSave.ActiveQuests)
	{
		const FPrimaryAssetId& QuestId = QuestData.QuestId;
		UQuestDataAsset* QuestAsset = QuestAssetsById[QuestId];
		FActiveQuest ActiveQuest = FActiveQuest(QuestId, QuestAsset);

		for (int i = 0; i < QuestData.ActiveObjectives.Num(); ++i)
		{
			const FActiveQuestObjectiveSaveData& ObjectiveData = QuestData.ActiveObjectives[i];
			ActiveQuest.StartObjective(ObjectiveData.ObjectiveId, World);
			ActiveQuest.ProgressObjective(ObjectiveData.ObjectiveId, ObjectiveData.CurrentProgress);
		}
		
		ActiveQuestsById.Add(
			QuestId,
			ActiveQuest
		);
	}
}

FQuestSaveData UQuestServiceImpl::GetQuestSave() const
{
	FQuestSaveData QuestSaveData;
	QuestSaveData.ActiveQuests = TArray<FActiveQuestSaveData>();
	QuestSaveData.CompletedQuests = TArray<FPrimaryAssetId>();

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

		QuestSaveData.ActiveQuests.Add(ActiveQuestData);
	}
	
	for (const FPrimaryAssetId& CompletedQuestId : CompletedQuestIds)
	{
		QuestSaveData.CompletedQuests.Add(CompletedQuestId);
	}

	return QuestSaveData;
}

void UQuestServiceImpl::CompleteQuest(const FPrimaryAssetId& QuestId)
{
	if (!QuestAssetsById.Contains(QuestId) || CompletedQuestIds.Contains(QuestId))
		return;

	if (ActiveQuestsById.Contains(QuestId))
	{
		ActiveQuestsById.Remove(QuestId);
	}
	
	CompletedQuestIds.Add(QuestId);

	UE_LOG(LogTemp, Display, TEXT("Quest %s completed."), *QuestId.ToString());

	bool _ = QuestCompletedDelegate.ExecuteIfBound(GetQuestDescription(QuestId));
}

void UQuestServiceImpl::OnQuestsLoaded(const FQuestLoadedDelegate& CompletionDelegate)
{
	const UAssetManager& AssetManager = UAssetManager::Get();
	TArray<FPrimaryAssetId> LoadedAssetIds;
	AssetManager.GetPrimaryAssetIdList(QuestAssetType, LoadedAssetIds);

	for (const FPrimaryAssetId& AssetId : LoadedAssetIds)
	{
		UObject* LoadedObj = AssetManager.GetPrimaryAssetObject(AssetId);
		if (UQuestDataAsset* Quest = Cast<UQuestDataAsset>(LoadedObj))
		{
			QuestAssetsById.Add(AssetId, Quest);
			UE_LOG(LogTemp, Log, TEXT("Loaded Quest: %s"), *Quest->GetName());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Quest assets loaded."));

	bool _ = CompletionDelegate.ExecuteIfBound();
}