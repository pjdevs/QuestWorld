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

void UQuestServiceImpl::StartQuest(const FName& QuestId, UWorld* World, APlayerController* PlayerController)
{
	UE_LOG(LogTemp, Display, TEXT("Starting Quest %s."), *QuestId.ToString());

	const TObjectPtr<UQuestDataAsset>* QuestDataAssetPtr = QuestAssetsById.Find(QuestId);

	if (QuestDataAssetPtr == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Quest not found."));
		return;
	}
	
	const FActiveQuest ActiveQuest(QuestId);
	// TODO Fill other quest data

	ActiveQuestsById.Add(QuestId, ActiveQuest);

	const UQuestDataAsset* QuestDataAsset = *QuestDataAssetPtr;

	if (QuestDataAsset->IsCompleted(World, PlayerController)) // TODO Move this to active quest
	{
		CompleteQuest(QuestId);
		return;
	}
	
	UE_LOG(LogTemp, Display, TEXT("Quest started."));
}

TArray<FName> UQuestServiceImpl::GetActiveQuests()
{
	TArray<FName> QuestIds;

	for (auto& Quest : ActiveQuestsById)
	{
		QuestIds.Add(Quest.Key);
	}
	
	return QuestIds;
}

TArray<FName> UQuestServiceImpl::GetCompletedQuests()
{
	return CompletedQuestIds;
}

void UQuestServiceImpl::CompleteQuest(const FName& QuestId)
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
			QuestAssetsById.Add(Quest->GetQuestId(), Quest);
			UE_LOG(LogTemp, Log, TEXT("Loaded Quest: %s"), *Quest->GetName());
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Quest assets loaded."));

	bool _ = CompletionDelegate.ExecuteIfBound();
}