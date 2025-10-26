// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveWorldSaveData.h"
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
	FInteractiveWorldSaveData InteractiveWorldSaveData = FInteractiveWorldSaveData();
};
