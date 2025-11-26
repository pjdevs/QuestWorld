// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestPhaseList.h"

#include "Assets/QuestDataAsset.h"

#if WITH_EDITOR
void FQuestPhaseList::OnOwningQuestChanged(const UQuestDataAsset* OwningQuestAsset)
{
	FGameplayTagContainer ValidPhases;

	if (OwningQuestAsset)
	{
		for (const FGameplayTag& Tag : Phases)
		{
			if (OwningQuestAsset->Phases.Contains(Tag))
			{
				ValidPhases.AddTag(Tag);
			}
		}
	}

	Phases = ValidPhases;
}
#endif
