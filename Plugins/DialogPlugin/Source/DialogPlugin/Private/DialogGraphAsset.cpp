// Copyright pjdevs. All Rights Reserved.


#include "DialogGraphAsset.h"
#include "PrimaryAssetTypes.h"

FPrimaryAssetId UDialogGraphAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetTypes::Dialog, GetFName());
}
