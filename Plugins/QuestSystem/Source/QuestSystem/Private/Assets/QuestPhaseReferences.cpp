// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestPhaseReferences.h"

#include "Assets/QuestDataAsset.h"

#if WITH_EDITOR
void FQuestPhaseReferences::OnOwningQuestChanged(const UQuestDataAsset* OwningQuestAsset)
{
	TSet<FName> ValidPhases;

	if (OwningQuestAsset)
	{
		for (const FName& Phase : Phases)
		{
			if (OwningQuestAsset->Phases.Contains(Phase))
			{
				ValidPhases.Add(Phase);
			}
		}
	}

	Phases = ValidPhases;
}
#endif
