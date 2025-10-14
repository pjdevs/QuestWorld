// Copyright pjdevs. All Rights Reserved.


#include "InventoryItemId.h"
#include "PrimaryAssetTypes.h"


FInventoryItemId::FInventoryItemId()
	: FInventoryItemId(NAME_None)
{
}

FInventoryItemId::FInventoryItemId(const FName& ItemName)
	: InventoryItemAssetId(PrimaryAssetTypes::InventoryItem, ItemName)
{
		
}

bool FInventoryItemId::IsValid() const
{
	return InventoryItemAssetId.IsValid() && InventoryItemAssetId.PrimaryAssetType == PrimaryAssetTypes::InventoryItem;
}
