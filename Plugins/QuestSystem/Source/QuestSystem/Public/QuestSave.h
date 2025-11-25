// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FlowSave.h"
#include "GameplayTags.h"
#include "QuestId.h"
#include "QuestSave.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FQuestObjectiveSateSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FGameplayTag ObjectiveId = FGameplayTag::EmptyTag;

	UPROPERTY(SaveGame)
	int CurrentProgress = 0;

	UPROPERTY(SaveGame)
	bool bIsCompleted = false;
};

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FQuestStateSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FQuestId QuestId = FQuestId();

	UPROPERTY(SaveGame)
	TArray<FQuestObjectiveSateSaveData> ObjectiveStates = TArray<FQuestObjectiveSateSaveData>();

	UPROPERTY(SaveGame)
	FFlowAssetSaveData QuestFlowSave = FFlowAssetSaveData();

	UPROPERTY(SaveGame)
	bool bIsCompleted;
};

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TArray<FQuestStateSaveData> QuestStates = TArray<FQuestStateSaveData>();
};
