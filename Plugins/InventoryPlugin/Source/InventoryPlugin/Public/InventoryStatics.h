// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryStatics.generated.h"

class UInventoryItemDataAsset;

/**
 * 
 */
UCLASS()
class INVENTORYPLUGIN_API UInventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	static UInventoryItemDataAsset* GetItem(FPrimaryAssetId ItemId);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	static bool IsItemExisting(FPrimaryAssetId ItemId);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	static int GetTotalItemCountForAllPlayers(const UWorld* World, FPrimaryAssetId ItemId);
};
