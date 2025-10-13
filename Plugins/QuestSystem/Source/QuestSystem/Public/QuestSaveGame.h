// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QuestSaveGame.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FActiveQuestObjectiveSaveData
{
	GENERATED_BODY()

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
	TArray<FActiveQuestObjectiveSaveData> Objectives = TArray<FActiveQuestObjectiveSaveData>();

	UPROPERTY()
	int CurrentObjectiveIndex = 0;
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

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class QUESTSYSTEM_API UQuestSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FQuestSaveData QuestSaveData = FQuestSaveData();
};
