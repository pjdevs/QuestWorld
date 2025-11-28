// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCompletionState.h"
#include "QuestId.h"
#include "QuestObjectiveState.h"
#include "QuestState.generated.h"

class UQuestDataAsset;

DECLARE_DELEGATE_TwoParams(FObjectiveDelegate, const FName& /* ObjectiveId */, EQuestObjectiveCompletionState /* CompletionState */)

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
	EQuestCompletionState GetCompletionState() const { return State; }
	bool IsCompleted() const { return State != EQuestCompletionState::Started; }
	
	const TMap<FName, FQuestObjectiveState>& GetObjectives() const { return ObjectiveStates; }
	bool IsObjectiveCompleted(const FName& ObjectiveId) const;
	bool IsObjectiveSucceeded(const FName& ObjectiveId) const;
	bool IsObjectiveFailed(const FName& ObjectiveId) const;

	void StartObjective(const FName& ObjectiveId, UWorld* World);
	void CompleteObjective(const FName& ObjectiveId, EQuestObjectiveCompletionState CompletionState);
	void ProgressObjective(const FName& ObjectiveId, int Progress);

	void SetCompletionState(EQuestCompletionState CompletionState);
	
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
	void HandleObjectiveCompletion(const FQuestObjectiveState& ObjectiveState);
	void CompleteQuestIfAllObjectivesCompleted(); 
	
private:
	FQuestId QuestId;

	UPROPERTY()
	TObjectPtr<const UQuestDataAsset> QuestAsset;

	UPROPERTY()
	TMap<FName, const TObjectPtr<const UQuestObjective>> ObjectiveAssets;

	TMap<FName, FQuestObjectiveState> ObjectiveStates;
	
	EQuestCompletionState State;
};
