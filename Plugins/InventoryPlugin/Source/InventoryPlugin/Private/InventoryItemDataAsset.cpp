// Copyright pjdevs. All Rights Reserved.


#include "InventoryItemDataAsset.h"
#include "PrimaryAssetTypes.h"

FPrimaryAssetId UInventoryItemDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetTypes::InventoryItem, ItemName);
}
