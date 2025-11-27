// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FlowSave.h"
#include "QuestId.h"
#include "QuestObjectiveCompletionState.h"
#include "QuestSave.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FQuestObjectiveSateSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FName ObjectiveId = NAME_None;

	UPROPERTY(SaveGame)
	int CurrentProgress = 0;

	UPROPERTY(SaveGame)
	EQuestObjectiveCompletionState CompletionState = EQuestObjectiveCompletionState::Started;
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
	EQuestCompletionState CompletionState = EQuestCompletionState::Started;
};

USTRUCT(Blueprintable, BlueprintType)
struct QUESTSYSTEM_API FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TArray<FQuestStateSaveData> QuestStates = TArray<FQuestStateSaveData>();
};
