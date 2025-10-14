// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Assets/BaseQuestEvent.h"
#include "InventoryQuestEvent.generated.h"


/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UInventoryQuestEvent : public UBaseQuestEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest, meta = (ExposeOnSpawn = "true"))
	FInventoryItemId ItemId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Quest, meta = (ExposeOnSpawn = "true"))
	int NewItemCount;
};
