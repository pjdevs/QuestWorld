// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestId.h"
#include "QuestObjectiveState.h"
#include "QuestState.generated.h"

class UQuestDataAsset;

DECLARE_DELEGATE_OneParam(FObjectiveDelegate, const FGameplayTag& /* ObjectiveId */)

/**
 * Runtime representation of a currently active / started / in-progress or completed quest.
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestState
{
	GENERATED_BODY()

public:
	FQuestState() = default; // Needed for creating TArray etc. Will see if we use TUniquePtr or so later
	FQuestState(const FQuestId& QuestId, const UQuestDataAsset* QuestAsset);

	const FQuestId& GetQuestId() const { return QuestId; }
	const UQuestDataAsset* GetQuestAsset() const { return QuestAsset; }
	bool IsCompleted() const { return bQuestCompleted; }
	
	const TMap<FGameplayTag, FQuestObjectiveState>& GetObjectives() const { return ObjectiveStates; }
	bool IsObjectiveCompleted(const FGameplayTag& ObjectiveId) const;
	
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

public:
	FObjectiveDelegate OnObjectiveCompleted;

private:
	void CompleteQuestIfAllObjectivesCompleted(); 
	
private:
	FQuestId QuestId;

	UPROPERTY()
	TObjectPtr<const UQuestDataAsset> QuestAsset;

	UPROPERTY()
	TMap<FGameplayTag, const TObjectPtr<const UQuestObjective>> ObjectiveAssets;

	TMap<FGameplayTag, FQuestObjectiveState> ObjectiveStates;
	
	bool bQuestCompleted;
};
