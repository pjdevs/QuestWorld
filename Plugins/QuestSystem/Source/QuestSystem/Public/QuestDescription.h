// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestDescription.generated.h"

/**
 * Description of a quest objective.
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestObjectiveDescription
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Quest)
	FText Description;

	UPROPERTY(BlueprintReadOnly, Category = Quest)
	int CurrentValue;

	UPROPERTY(BlueprintReadOnly, Category = Quest)
	int TargetValue;

	UPROPERTY(BlueprintReadOnly, Category = Quest)
	bool bIsCompleted;
};

/**
 * Description of a quest. Potentially to be user in UI.
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestDescription
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = Quest)
	FPrimaryAssetId QuestId;

	UPROPERTY(BlueprintReadOnly, Category = Quest)
	FText Title;

	UPROPERTY(BlueprintReadOnly, Category = Quest)
	FText Description;

	UPROPERTY(BlueprintReadOnly, Category = Quest)
	TArray<FQuestObjectiveDescription> Objectives;

	UPROPERTY(BlueprintReadOnly, Category = Quest)
	bool bIsCompleted;
};
