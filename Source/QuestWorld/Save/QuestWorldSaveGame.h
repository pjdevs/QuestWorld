// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPWorldStateSaveData.h"
#include "InventorySaveData.h"
#include "QuestSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "QuestWorldSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class QUESTWORLD_API UQuestWorldSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FQuestSaveData QuestSaveData = FQuestSaveData();

	UPROPERTY()
	FInventorySaveData InventorySaveData = FInventorySaveData();

	UPROPERTY()
	FIPWorldStateSaveData InteractiveWorldSaveData = FIPWorldStateSaveData();
};
