// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestDataAsset.h"

#include "PrimaryAssetTypes.h"


FPrimaryAssetId UQuestDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(PrimaryAssetTypes::Quest, GetFName());
}

#if WITH_EDITOR
void UQuestDataAsset::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UQuestDataAsset, Phases))
	{
		for (UQuestObjective* Objective : Objectives)
		{
			Objective->Phases.OnOwningQuestChanged(this);
		}
	}
}
#endif