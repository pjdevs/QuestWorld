﻿// Copyright pjdevs. All Rights Reserved.

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

	FActiveQuestObjective() = default; // Needed for creating TArray etc. Will see if we use TUniquePtr or so later
	FActiveQuestObjective(UQuestObjective* ObjectiveAsset, UWorld* World);

	int GetCurrentProgress() const { return CurrentProgress; }
	bool IsObjectiveCompleted() const { return CurrentProgress >= ObjectiveAsset->GetTargetValue(); }
	void OnQuestEvent(UWorld* World, UBaseQuestEvent* Event);

private:
	UPROPERTY()
	TObjectPtr<UQuestObjective> ObjectiveAsset;

	int CurrentProgress;
};
