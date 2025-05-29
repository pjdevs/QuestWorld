// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveQuest.h"
#include "QuestService.generated.h"

class UQuestDataAsset;

DECLARE_DELEGATE(FQuestLoadedDelegate);

// Do not modify
UINTERFACE()
class UQuestService : public UInterface
{
	GENERATED_BODY()
};


/**
 * 
 */
class QUESTSYSTEM_API IQuestService
{
	GENERATED_BODY()
	
public:
	virtual void LoadQuests(FQuestLoadedDelegate CompletionDelegate = FQuestLoadedDelegate()) = 0;
	virtual void StartQuest(const FPrimaryAssetId& QuestId, UWorld* World) = 0;
	virtual TArray<FPrimaryAssetId> GetActiveQuests() = 0;
	virtual TArray<FPrimaryAssetId> GetCompletedQuests() = 0;
};

/**
 * 
 */
UCLASS()
class UQuestServiceImpl final : public UObject, public IQuestService
{
	GENERATED_BODY()
	
public:
	UQuestServiceImpl();

	virtual void LoadQuests(FQuestLoadedDelegate CompletionDelegate) override;
	virtual void StartQuest(const FPrimaryAssetId& QuestId, UWorld* World) override;
	virtual TArray<FPrimaryAssetId> GetActiveQuests() override;
	virtual TArray<FPrimaryAssetId> GetCompletedQuests() override;

protected:
	void CompleteQuest(const FPrimaryAssetId& QuestId);
	void OnQuestsLoaded(const FQuestLoadedDelegate& CompletionDelegate);
	
private:
	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<UQuestDataAsset>> QuestAssetsById;

	TMap<FPrimaryAssetId, FActiveQuest> ActiveQuestsById;
	TArray<FPrimaryAssetId> CompletedQuestIds;

	FPrimaryAssetType QuestAssetType;
};
