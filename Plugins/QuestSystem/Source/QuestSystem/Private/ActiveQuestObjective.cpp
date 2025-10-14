// Copyright pjdevs. All Rights Reserved.


#include "ActiveQuestObjective.h"
#include "Assets/QuestObjective.h"

FActiveQuestObjective::FActiveQuestObjective(UQuestObjective* ObjectiveAsset, UWorld* World)
	: ObjectiveAsset(ObjectiveAsset), CurrentProgress(0)
{
	ensureMsgf(World != nullptr, TEXT("World should not be null"));
	ensureMsgf(ObjectiveAsset != nullptr, TEXT("ObjectiveAsset should not be null"));

	if (ObjectiveAsset->IsRetroCompletable())
	{
		CurrentProgress = ObjectiveAsset->GetCompletion(World);
	}
}

bool FActiveQuestObjective::OnQuestEvent(UWorld* World, UBaseQuestEvent* Event)
{
	const int Progress = ObjectiveAsset->TriggerProgress(World, Event);

	if (ObjectiveAsset->ShouldAddProgress())
	{
		CurrentProgress += Progress;
	}
	else
	{
		CurrentProgress = Progress;
	}

	return Progress > 0;
}
