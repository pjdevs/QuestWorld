// Copyright pjdevs. All Rights Reserved.


#include "ActiveQuest.h"

#include "ActiveQuestObjective.h"
#include "Assets/QuestDataAsset.h"
#include "Assets/QuestStep.h"


FActiveQuest::FActiveQuest(
	const FPrimaryAssetId& QuestId,
	UQuestDataAsset* QuestDataAsset,
	UWorld* World,
	int StepIndex
)
	: QuestId(QuestId), QuestDataAsset(QuestDataAsset), CurrentStepIndex(StepIndex)
{
	LoadStep(CurrentStepIndex, World);
}

FActiveQuest::FActiveQuest(const FPrimaryAssetId& QuestId, UQuestDataAsset* QuestDataAsset, UWorld* World)
	: FActiveQuest(QuestId, QuestDataAsset, World, 0)
{
}

bool FActiveQuest::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	if (IsCompleted())
	{
		return false;
	}
	
	bool bAreAllObjectivesCompleted = true; 
	bool bHasAnyObjectiveProgressed = false;
	
	for (auto& ActiveQuestObjective : Objectives)
	{
		if (ActiveQuestObjective.IsObjectiveCompleted())
		{
			continue;
		}
		
		bHasAnyObjectiveProgressed |= ActiveQuestObjective.OnQuestEvent(World, Event);
		bAreAllObjectivesCompleted = bAreAllObjectivesCompleted && ActiveQuestObjective.IsObjectiveCompleted();
	}

	if (bAreAllObjectivesCompleted)
	{
		LoadStep(CurrentStepIndex + 1, World);
		return true;
	}

	return bHasAnyObjectiveProgressed;
}

void FActiveQuest::LoadStep(int StepIndex, UWorld* World)
{
	CurrentStepIndex = StepIndex;

	if (IsCompleted())
	{
		return;
	}

	bool bAllObjectivesCompleted = true;
	const UQuestStep* CurrentQuestStep = QuestDataAsset->GetQuestSteps()[CurrentStepIndex];
	
	for (const TObjectPtr<UQuestObjective>& ObjectiveAsset : CurrentQuestStep->GetObjectives())
	{
		FActiveQuestObjective ActiveQuestObjective(ObjectiveAsset, World);
		Objectives.Add(ActiveQuestObjective);

		bAllObjectivesCompleted &= ActiveQuestObjective.IsObjectiveCompleted();
	}

	if (bAllObjectivesCompleted)
	{
		LoadStep(CurrentStepIndex + 1, World);
	}
}
