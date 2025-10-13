// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestStep.generated.h"

class UQuestObjective;

/**
 * A step containing its own subset of objectives inside a quest data asset.
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class QUESTSYSTEM_API UQuestStep : public UObject
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	FName StepName;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Quest", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UQuestObjective>> Objectives;

public:
	const FName& GetStepName() const { return StepName; }
	const TArray<TObjectPtr<UQuestObjective>>& GetObjectives() const { return Objectives; }
};
