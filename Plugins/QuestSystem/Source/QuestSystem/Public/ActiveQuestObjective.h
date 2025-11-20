// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Assets/QuestObjective.h"
#include "ActiveQuestObjective.generated.h"


class UBaseQuestEvent;
class UQuestObjective;

USTRUCT()
struct QUESTSYSTEM_API FActiveQuestObjective
{
	GENERATED_BODY()

public:
	FActiveQuestObjective() = default; // Needed for creating TArray etc. Will see if we use TUniquePtr or so later
	FActiveQuestObjective(UQuestObjective* ObjectiveAsset);

	FGameplayTag GetObjectiveId() const { return ObjectiveAsset->ObjectiveId; }
	bool IsRetroCompletable() const { return ObjectiveAsset->bIsRetroCompletable; } 
	bool IsObjectiveCompleted() const { return bIsObjectiveCompleted; }

	int GetCurrentProgress() const { return CurrentProgress; }
	void SetCurrentProgress(int Progress);
	void ProgressObjective(int Progress);
	void CompleteObjective();
	
	bool OnQuestEvent(UWorld* World, UBaseQuestEvent* Event);

private:
	UPROPERTY()
	TObjectPtr<UQuestObjective> ObjectiveAsset;

	int CurrentProgress;
	bool bIsObjectiveCompleted;
};
