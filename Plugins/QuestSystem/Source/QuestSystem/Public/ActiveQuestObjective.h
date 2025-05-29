// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ActiveQuestObjective.generated.h"


class UQuestObjective;

USTRUCT()
struct QUESTSYSTEM_API FActiveQuestObjective
{
	GENERATED_BODY()

	explicit FActiveQuestObjective(UQuestObjective* ObjectiveAsset, UWorld* World);

	bool IsObjectiveCompleted() const { return bObjectiveCompleted; }
	void OnQuestEvent(const FName& EventType, AActor* EventActor, UWorld* World);

private:
	void ReEvaluateQuestObjective(UWorld* World);
	
private:
	UPROPERTY()
	TObjectPtr<UQuestObjective> ObjectiveAsset;

	bool bObjectiveCompleted;
};
