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
	virtual void StartQuest(const FName& QuestId, UWorld* World, APlayerController* PlayerController) = 0;
	virtual TArray<FName> GetActiveQuests() = 0;
	virtual TArray<FName> GetCompletedQuests() = 0;
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
	virtual void StartQuest(const FName& QuestId, UWorld* World, APlayerController* PlayerController) override;
	virtual TArray<FName> GetActiveQuests() override;
	virtual TArray<FName> GetCompletedQuests() override;

protected:
	void CompleteQuest(const FName& QuestId);
	void OnQuestsLoaded(const FQuestLoadedDelegate& CompletionDelegate);
	
private:
	UPROPERTY()
	TMap<FName, TObjectPtr<UQuestDataAsset>> QuestAssetsById;

	TMap<FName, FActiveQuest> ActiveQuestsById;
	TArray<FName> CompletedQuestIds;

	FPrimaryAssetType QuestAssetType;
};
