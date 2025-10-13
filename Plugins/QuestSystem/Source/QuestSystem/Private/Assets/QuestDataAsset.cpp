#include "Assets/QuestDataAsset.h"

#include "PrimaryAssetTypes.h"


FPrimaryAssetId UQuestDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetTypes::Quest, GetFName());
}
