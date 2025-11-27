// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjectiveCompletionState.h"
#include "Assets/QuestObjective.h"
#include "QuestObjectiveState.generated.h"


class UBaseQuestEvent;
class UQuestObjective;

USTRUCT()
struct QUESTSYSTEM_API FQuestObjectiveState
{
	GENERATED_BODY()

public:
	FQuestObjectiveState() = default; // Needed for creating TArray etc. Will see if we use TUniquePtr or so later
	FQuestObjectiveState(const UQuestObjective* ObjectiveAsset);

	const FName& GetObjectiveId() const { return ObjectiveAsset->ObjectiveId; }
	const FText& GetDescription() const { return ObjectiveAsset->ObjectiveDescription; }
	bool IsRetroCompletable() const { return ObjectiveAsset->bIsRetroCompletable; } 
	EQuestObjectiveCompletionState GetCompletionState() const { return State; }
	bool IsCompleted() const { return State != EQuestObjectiveCompletionState::Started; }
	bool IsOptional() const { return ObjectiveAsset->bIsOptional; }

	int GetCurrentProgress() const { return CurrentProgress; }
	int GetTargetProgress() const { return ObjectiveAsset->GetTargetValue(); }
	void SetCurrentProgress(int Progress);
	void ProgressObjective(int Progress);
	void SetCompletionState(EQuestObjectiveCompletionState CompletionState);
	
	bool OnQuestEvent(UWorld* World, UBaseQuestEvent* Event);

private:
	UPROPERTY()
	TObjectPtr<const UQuestObjective> ObjectiveAsset;

	EQuestObjectiveCompletionState State;
	int CurrentProgress;
};
