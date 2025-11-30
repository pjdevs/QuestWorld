// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "QuestCompletionState.h"
#include "QuestObjectiveCompletionState.h"
#include "QuestId.h"
#include "QuestDescription.generated.h"

/**
 * Description of a quest objective.
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestObjectiveDescription
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	FText Description = FText();

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	int CurrentValue = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	int TargetValue = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	EQuestObjectiveCompletionState CompletionState = EQuestObjectiveCompletionState::Started;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	bool bIsOptional = false;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	bool bIsActiveInPhase = false;
};

/**
 * Description of a quest. Display only info to potentially be used in UI.
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestDescription
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	FQuestId QuestId = FQuestId();

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	FGameplayTag QuestType;

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	FText Title = FText();

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	FText Description = FText();

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TArray<FQuestObjectiveDescription> Objectives = TArray<FQuestObjectiveDescription>();

	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	EQuestCompletionState CompletionState = EQuestCompletionState::Started;
};
