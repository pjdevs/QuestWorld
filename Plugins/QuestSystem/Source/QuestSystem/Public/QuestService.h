// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveQuest.h"
#include "QuestService.generated.h"

struct FQuestDescription;
class UBaseQuestEvent;
class UQuestDataAsset;

DECLARE_DELEGATE(FQuestLoadedDelegate);
DECLARE_DELEGATE_OneParam(FQuestCompletedDelegate, FPrimaryAssetId);

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
	virtual TArray<FPrimaryAssetId> GetActiveQuests() const = 0;
	virtual TArray<FPrimaryAssetId> GetCompletedQuests() const = 0;
	virtual void SubmitQuestEvent(UWorld* World, UBaseQuestEvent* Event) = 0;

	virtual FQuestDescription GetQuestDescription(const FPrimaryAssetId& QuestId) const = 0;
	virtual TArray<FQuestDescription> GetActiveQuestDescriptions() const = 0;
	virtual TArray<FQuestDescription> GetCompletedQuestDescriptions() const = 0;

	virtual void SetQuestCompletedDelegate(const FQuestCompletedDelegate& InQuestCompletedDelegate) = 0;
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
	virtual TArray<FPrimaryAssetId> GetActiveQuests() const override;
	virtual TArray<FPrimaryAssetId> GetCompletedQuests() const override;
	virtual void SubmitQuestEvent(UWorld* World, UBaseQuestEvent* Event) override;

	virtual TArray<FQuestDescription> GetActiveQuestDescriptions() const override;
	virtual TArray<FQuestDescription> GetCompletedQuestDescriptions() const override;
	virtual FQuestDescription GetQuestDescription(const FPrimaryAssetId& QuestId) const override;

	virtual void SetQuestCompletedDelegate(const FQuestCompletedDelegate& InQuestCompletedDelegate) override;

protected:
	void CompleteQuest(const FPrimaryAssetId& QuestId);
	void OnQuestsLoaded(const FQuestLoadedDelegate& CompletionDelegate);
	
private:
	FPrimaryAssetType QuestAssetType;

	UPROPERTY()
	TMap<FPrimaryAssetId, TObjectPtr<UQuestDataAsset>> QuestAssetsById;

	TMap<FPrimaryAssetId, FActiveQuest> ActiveQuestsById;
	TArray<FPrimaryAssetId> CompletedQuestIds;

	FQuestCompletedDelegate QuestCompletedDelegate;
};
