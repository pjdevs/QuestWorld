// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestPhaseReferences.h"

#include "Assets/QuestDataAsset.h"


FString FQuestPhaseReferences::ToString() const
{
	FString SummaryString;
	int Count = 0;

	for (const FName& Phase : Phases)
	{
		SummaryString += Phase.ToString();

		if (Count < Phases.Num() - 1)
		{
			SummaryString += "\n";
		}

		++Count;
	}

	return SummaryString;
}

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
