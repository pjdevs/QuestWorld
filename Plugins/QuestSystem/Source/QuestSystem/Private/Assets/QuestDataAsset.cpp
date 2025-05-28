#include "Assets/QuestDataAsset.h"

#include "PrimaryAssetTypes.h"

bool UQuestDataAsset::IsCompleted(UWorld* World, APlayerController* PlayerController) const
{
    bool bCompleted = false;

    for (int i = 0; i < Objectives.Num(); i++)
    {
    	if (Objectives[i] == nullptr)
    		continue;

    	bCompleted = bCompleted || Objectives[i]->IsSatisfied(World, PlayerController);
    }

    return bCompleted;
}

FPrimaryAssetId UQuestDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetTypes::Quest, GetFName());
}
