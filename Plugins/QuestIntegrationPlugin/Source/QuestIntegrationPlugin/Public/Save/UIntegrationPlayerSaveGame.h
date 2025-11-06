// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Save/GaspPlayerSaveData.h"
#include "UIntegrationPlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UUIntegrationPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FGaspPlayerSaveData GaspSaveData = FGaspPlayerSaveData();

	UPROPERTY()
	FInventorySaveData InventorySaveData = FInventorySaveData();
};
