// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryStatics.generated.h"

class UInventoryComponent;
struct FInventoryItemId;
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
	static UInventoryItemDataAsset* GetItem(FInventoryItemId ItemId);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	static bool IsItemExisting(FInventoryItemId ItemId);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	static int GetTotalItemCountForAllPlayers(const UWorld* World, FInventoryItemId ItemId);

	UFUNCTION(BlueprintCallable, Category = Inventory, meta=(WorldContext="WorldContextObject"))
	static UInventoryComponent* GetSharedInventory(UObject* WorldContextObject);
};
