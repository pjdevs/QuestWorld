// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FlowSave.h"
#include "GameplayTags.h"
#include "QuestId.h"
#include "QuestSaveGame.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FActiveQuestObjectiveSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FGameplayTag ObjectiveId = FGameplayTag::EmptyTag;

	UPROPERTY(SaveGame)
	int CurrentProgress = 0;
};

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FActiveQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FQuestId QuestId = FQuestId();

	UPROPERTY(SaveGame)
	TArray<FActiveQuestObjectiveSaveData> ActiveObjectives = TArray<FActiveQuestObjectiveSaveData>();

	UPROPERTY(SaveGame)
	FFlowAssetSaveData QuestFlowSave = FFlowAssetSaveData();
};

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TArray<FActiveQuestSaveData> ActiveQuests = TArray<FActiveQuestSaveData>();

	UPROPERTY(SaveGame)
	TArray<FQuestId> CompletedQuests = TArray<FQuestId>();
};
