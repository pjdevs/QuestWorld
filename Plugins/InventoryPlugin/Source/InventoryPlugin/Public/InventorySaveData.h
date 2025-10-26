// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemId.h"
#include "InventorySaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct INVENTORYPLUGIN_API FInventoryEntrySaveData
{
	GENERATED_BODY()

	UPROPERTY()
	FInventoryItemId ItemId = FInventoryItemId();

	UPROPERTY()
	int Quantity = 0;
};

USTRUCT(Blueprintable, BlueprintType)
struct INVENTORYPLUGIN_API FInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventoryEntrySaveData> Items = TArray<FInventoryEntrySaveData>();
};
