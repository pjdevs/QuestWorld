// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveQuestObjective.h"
#include "QuestSaveGame.h"
#include "ActiveQuest.generated.h"

class UQuestDataAsset;

/**
 * Runtime representation of a currently active / started / in-progress quest.
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FActiveQuest
{
	GENERATED_BODY()

public:
	FActiveQuest() = default; // Needed for creating TArray etc. Will see if we use TUniquePtr or so later
	FActiveQuest(const FPrimaryAssetId& QuestId, UQuestDataAsset* QuestDataAsset);

	const FPrimaryAssetId& GetQuestId() const { return QuestId; }
	bool IsCompleted() const { return bQuestCompleted; }

	const FActiveQuestObjective* GetActiveObjective(const FGameplayTag& ObjectiveId) const { return ActiveObjectives.Find(ObjectiveId); }
	const TMap<FGameplayTag, FActiveQuestObjective>& GetActiveObjectives() const { return ActiveObjectives; }
	bool IsObjectiveCompleted(const FGameplayTag& ObjectiveId) const { return CompletedObjectives.Contains(ObjectiveId); }
	
	void StartObjective(const FGameplayTag& ObjectiveId, UWorld* World);
	void CompleteObjective(const FGameplayTag& ObjectiveId);
	void ProgressObjective(const FGameplayTag& ObjectiveId, int Progress);

	void CompleteQuest();
	
	/**
	 * Notify the quest of an emitted quest event.
	 * @param World 
	 * @param Event 
	 * @return true if the event made some progression or false else.
	 */
	bool OnQuestEvent(UWorld* World, UBaseQuestEvent* Event);
	
private:
	UPROPERTY()
	TObjectPtr<UQuestDataAsset> QuestDataAsset;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UQuestObjective>> ObjectiveAssets;

	FPrimaryAssetId QuestId;
	TMap<FGameplayTag, FActiveQuestObjective> ActiveObjectives;
	TSet<FGameplayTag> CompletedObjectives;
	bool bQuestCompleted;
};
