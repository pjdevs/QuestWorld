// Copyright pjdevs. All Rights Reserved.


#include "Assets/ActionQuestObjective.h"

#include "Assets/ActionQuestEvent.h"

UActionQuestObjective::UActionQuestObjective()
	: ActionToDo(FName("None")), TimesToDoAction(1)
{
	bIsRetroCompletable = false;
	bShouldAddProgress = true;
}

int UActionQuestObjective::GetCompletion_Implementation(UWorld* World) const
{
	return 0;
}

int UActionQuestObjective::GetTargetValue_Implementation() const
{
	return TimesToDoAction;
}

int UActionQuestObjective::TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event) const
{
	if (const UActionQuestEvent* ActionEvent = Cast<UActionQuestEvent>(Event))
	{
		if (ActionEvent->ActionName == ActionToDo)
		{
			return 1;
		}
	}

	return 0;
}
