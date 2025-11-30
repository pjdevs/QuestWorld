// Copyright pjdevs. All Rights Reserved.


#include "QuestObjectiveState.h"
#include "Assets/QuestObjective.h"

FQuestObjectiveState::FQuestObjectiveState(const UQuestObjective* ObjectiveAsset)
	: ObjectiveAsset(ObjectiveAsset), State(EQuestObjectiveCompletionState::Started), CurrentProgress(0)
{
	ensureMsgf(ObjectiveAsset != nullptr, TEXT("ObjectiveAsset should not be null"));
}

bool FQuestObjectiveState::IsActiveInPhase(const FName& Phase) const
{
	return ObjectiveAsset->Phases.Phases.IsEmpty() || ObjectiveAsset->Phases.Phases.Contains(Phase);
}

void FQuestObjectiveState::SetCurrentProgress(int Progress)
{
	CurrentProgress = Progress;

	if (CurrentProgress >= ObjectiveAsset->GetTargetValue())
	{
		State = EQuestObjectiveCompletionState::Succeeded;
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

void FQuestObjectiveState::SetCompletionState(EQuestObjectiveCompletionState CompletionState)
{
	State = CompletionState;
}

bool FQuestObjectiveState::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	const int OldProgress = CurrentProgress;
	const int Progress = ObjectiveAsset->TriggerProgress(World, Event);
	
	ProgressObjective(Progress);

	return GetCurrentProgress() > OldProgress;
}
