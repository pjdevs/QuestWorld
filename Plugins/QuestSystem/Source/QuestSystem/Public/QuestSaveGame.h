// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "QuestSaveGame.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FActiveQuestObjectiveSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag ObjectiveId = FGameplayTag::EmptyTag;

	UPROPERTY()
	int CurrentProgress = 0;
};

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FActiveQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FPrimaryAssetId QuestId = FPrimaryAssetId();

	UPROPERTY()
	TArray<FActiveQuestObjectiveSaveData> ActiveObjectives = TArray<FActiveQuestObjectiveSaveData>();
};

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FActiveQuestSaveData> ActiveQuests = TArray<FActiveQuestSaveData>();

	UPROPERTY()
	TArray<FPrimaryAssetId> CompletedQuests = TArray<FPrimaryAssetId>();
};
