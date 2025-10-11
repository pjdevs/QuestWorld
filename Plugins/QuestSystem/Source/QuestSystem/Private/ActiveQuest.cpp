// Copyright pjdevs. All Rights Reserved.


#include "ActiveQuest.h"

#include "ActiveQuestObjective.h"
#include "Assets/QuestDataAsset.h"


FActiveQuest::FActiveQuest(const FPrimaryAssetId& QuestId, UQuestDataAsset* QuestDataAsset, UWorld* World)
	: QuestId(QuestId), QuestDataAsset(QuestDataAsset), bQuestCompleted(false)
{
	bool bAllObjectivesCompleted = true; 
	
	for (auto& ObjectiveAsset : QuestDataAsset->GetQuestObjectives())
	{
		FActiveQuestObjective ActiveQuestObjective(ObjectiveAsset, World);
		Objectives.Add(ActiveQuestObjective);

		bAllObjectivesCompleted = bAllObjectivesCompleted && ActiveQuestObjective.IsObjectiveCompleted();
	}

	bQuestCompleted = bAllObjectivesCompleted;
}

FActiveQuest::~FActiveQuest()
{
}

bool FActiveQuest::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	if (bQuestCompleted)
		return false;
	
	bool bAllObjectivesCompleted = true; 
	bool bAnyObjectiveProgressed = false;
	
	for (auto& ActiveQuestObjective : Objectives)
	{
		if (ActiveQuestObjective.IsObjectiveCompleted())
			continue;
		
		bAnyObjectiveProgressed |= ActiveQuestObjective.OnQuestEvent(World, Event);

		bAllObjectivesCompleted = bAllObjectivesCompleted && ActiveQuestObjective.IsObjectiveCompleted();
	}

	bQuestCompleted = bAllObjectivesCompleted;

	return bAnyObjectiveProgressed;
}
