// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemId.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct INVENTORYPLUGIN_API FInventoryItemId
{
	GENERATED_BODY()
	
public:
	FInventoryItemId();
	FInventoryItemId(const FName& ItemName);

	bool IsValid() const;

	bool operator==(const FInventoryItemId& Other) const
	{
		return InventoryItemAssetId == Other.InventoryItemAssetId;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true, AllowedTypes = InventoryItem))
	FPrimaryAssetId InventoryItemAssetId;
};
