// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InventoryItemDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class INVENTORYPLUGIN_API UInventoryItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = Inventory)
	const FName& GetItemName() const { return ItemName; }

	UFUNCTION(BlueprintPure, Category = Inventory)
	UTexture2D* GetItemIcon() const { return ItemIcon; }
	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	TObjectPtr<UTexture2D> ItemIcon;
};
