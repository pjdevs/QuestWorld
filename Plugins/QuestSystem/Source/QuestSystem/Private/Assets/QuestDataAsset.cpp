#include "Assets/QuestDataAsset.h"

#include "PrimaryAssetTypes.h"

bool UQuestDataAsset::IsCompleted(UWorld* World) const
{
    bool bCompleted = false;

    for (int i = 0; i < Objectives.Num(); i++)
    {
    	if (Objectives[i] == nullptr)
    		continue;

    	bCompleted = bCompleted || Objectives[i]->GetCompletion(World);
    }

    return bCompleted;
}

FPrimaryAssetId UQuestDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetTypes::Quest, GetFName());
}
