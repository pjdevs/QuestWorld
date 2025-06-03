// Copyright pjdevs. All Rights Reserved.


#include "DialogDataAsset.h"

#include "PrimaryAssetTypes.h"

FPrimaryAssetId UDialogDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetTypes::Dialog, GetFName());
}
