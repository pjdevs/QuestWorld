// Copyright pjdevs. All Rights Reserved.


#include "ActiveQuestObjective.h"
#include "Assets/QuestObjective.h"

FActiveQuestObjective::FActiveQuestObjective(UQuestObjective* ObjectiveAsset)
	: ObjectiveAsset(ObjectiveAsset), CurrentProgress(0), bIsObjectiveCompleted(false)
{
	ensureMsgf(ObjectiveAsset != nullptr, TEXT("ObjectiveAsset should not be null"));
}

void FActiveQuestObjective::SetCurrentProgress(int Progress)
{
	CurrentProgress = Progress;

	if (CurrentProgress >= ObjectiveAsset->GetTargetValue())
	{
		bIsObjectiveCompleted = true;
	}
}

void FActiveQuestObjective::ProgressObjective(int Progress)
{
	if (ObjectiveAsset->bShouldAddProgress)
	{
		SetCurrentProgress(CurrentProgress + Progress);
	}
	else
	{
		SetCurrentProgress(Progress);
	}
}

void FActiveQuestObjective::CompleteObjective()
{
	bIsObjectiveCompleted = true;
}

bool FActiveQuestObjective::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	const int OldProgress = CurrentProgress;
	const int Progress = ObjectiveAsset->TriggerProgress(World, Event);
	
	ProgressObjective(Progress);

	return GetCurrentProgress() > OldProgress;
}
