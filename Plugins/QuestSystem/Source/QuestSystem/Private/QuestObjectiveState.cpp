// Copyright pjdevs. All Rights Reserved.


#include "QuestObjectiveState.h"
#include "Assets/QuestObjective.h"

FQuestObjectiveState::FQuestObjectiveState(const UQuestObjective* ObjectiveAsset)
	: ObjectiveAsset(ObjectiveAsset), CurrentProgress(0), bIsCompleted(false)
{
	ensureMsgf(ObjectiveAsset != nullptr, TEXT("ObjectiveAsset should not be null"));
}

void FQuestObjectiveState::SetCurrentProgress(int Progress)
{
	CurrentProgress = Progress;

	if (CurrentProgress >= ObjectiveAsset->GetTargetValue())
	{
		bIsCompleted = true;
	}
}

void FQuestObjectiveState::ProgressObjective(int Progress)
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

void FQuestObjectiveState::CompleteObjective()
{
	bIsCompleted = true;
}

bool FQuestObjectiveState::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	const int OldProgress = CurrentProgress;
	const int Progress = ObjectiveAsset->TriggerProgress(World, Event);
	
	ProgressObjective(Progress);

	return GetCurrentProgress() > OldProgress;
}
