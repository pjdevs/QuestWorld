// Copyright pjdevs. All Rights Reserved.

#include "QuestService.h"

#include "ActiveQuest.h"
#include "PrimaryAssetTypes.h"
#include "Assets/QuestDataAsset.h"
#include "Engine/AssetManager.h"

UQuestServiceImpl::UQuestServiceImpl()
	: QuestAssetType(PrimaryAssetTypes::Quest)
{
	
}


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
	
	const FActiveQuest ActiveQuest(QuestId, *QuestDataAssetPtr, World);
	ActiveQuestsById.Add(QuestId, ActiveQuest);
	
	if (ActiveQuest.IsCompleted())
	{
		CompleteQuest(QuestId);
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Quest started."));
}

TArray<FPrimaryAssetId> UQuestServiceImpl::GetActiveQuests()
{
	TArray<FPrimaryAssetId> QuestIds;

	for (auto& Quest : ActiveQuestsById)
	{
		QuestIds.Add(Quest.Key);
	}
	
	return QuestIds;
}

TArray<FPrimaryAssetId> UQuestServiceImpl::GetCompletedQuests()
{
	return CompletedQuestIds;
}

void UQuestServiceImpl::SubmitQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	TArray<FPrimaryAssetId> QuestToCompleteIds;
	
	for (auto& Tuple : ActiveQuestsById)
	{
		FActiveQuest& ActiveQuest = Tuple.Value;
		ActiveQuest.OnQuestEvent(World, Event);
		
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