// Copyright pjdevs. All Rights Reserved.


#include "ActiveQuestObjective.h"

#include "Assets/QuestObjective.h"

FActiveQuestObjective::FActiveQuestObjective(UQuestObjective* ObjectiveAsset, UWorld* World)
	: ObjectiveAsset(ObjectiveAsset), bObjectiveCompleted(false)
{
	ensureMsgf(World != nullptr, TEXT("World should not be null"));
	ensureMsgf(ObjectiveAsset != nullptr, TEXT("ObjectiveAsset should not be null"));

	if (ObjectiveAsset->IsRetroCompletable())
	{
		ReEvaluateQuestObjective(World);
	}
}

void FActiveQuestObjective::OnQuestEvent(const FName& EventType, AActor* EventActor, UWorld* World)
{
	// TODO Only reevaluate if event type matching
	ReEvaluateQuestObjective(World);
}

void FActiveQuestObjective::ReEvaluateQuestObjective(UWorld* World)
{
	bObjectiveCompleted = ObjectiveAsset->IsSatisfied(World);
}
