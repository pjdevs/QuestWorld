// Copyright pjdevs. All Rights Reserved.


#include "ActiveQuest.h"
#include "ActiveQuestObjective.h"
#include "Assets/QuestDataAsset.h"


FActiveQuest::FActiveQuest(const FPrimaryAssetId& QuestId, UQuestDataAsset* QuestDataAsset, UWorld* World)
	: QuestDataAsset(QuestDataAsset), QuestId(QuestId),
	bQuestCompleted(false), bIsSequential(QuestDataAsset->IsSequential()), CurrentObjectiveIndex(0)
{
	bool bAllObjectivesCompleted = true; 
	
	for (auto& ObjectiveAsset : QuestDataAsset->GetQuestObjectives())
	{
		FActiveQuestObjective ActiveQuestObjective(ObjectiveAsset, World);
		Objectives.Add(ActiveQuestObjective);

		const bool bIsObjectiveCompleted = ActiveQuestObjective.IsObjectiveCompleted();

		if (bIsObjectiveCompleted && bIsSequential)
		{
			CurrentObjectiveIndex++;
		}
		
		bAllObjectivesCompleted &= bIsObjectiveCompleted;
	}

	bQuestCompleted = bAllObjectivesCompleted;
}

void FActiveQuest::RestoreCurrentObjectiveIndex(int ObjectiveIndex)
{
	CurrentObjectiveIndex = ObjectiveIndex;
}

bool FActiveQuest::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	if (bQuestCompleted)
	{
		return false;
	}
	
	bool bAnyObjectiveProgressed = false;

	if (bIsSequential)
	{
		FActiveQuestObjective& CurrentObjective = Objectives[CurrentObjectiveIndex];
		bAnyObjectiveProgressed |= CurrentObjective.OnQuestEvent(World, Event);

		const bool bIsObjectiveCompleted = CurrentObjective.IsObjectiveCompleted();
		
		if (bIsObjectiveCompleted)
		{
			CurrentObjectiveIndex++;
		}
		
		bQuestCompleted = bIsObjectiveCompleted && CurrentObjectiveIndex >= Objectives.Num();
	}
	else
	{
		bool bAllObjectivesCompleted = true;

		for (FActiveQuestObjective& ActiveQuestObjective : Objectives)
		{
			if (ActiveQuestObjective.IsObjectiveCompleted())
			{
				continue;
			}
			
			bAnyObjectiveProgressed |= ActiveQuestObjective.OnQuestEvent(World, Event);
			bAllObjectivesCompleted &= ActiveQuestObjective.IsObjectiveCompleted();
		}

		bQuestCompleted = bAllObjectivesCompleted;
	}

	return bAnyObjectiveProgressed;
}
