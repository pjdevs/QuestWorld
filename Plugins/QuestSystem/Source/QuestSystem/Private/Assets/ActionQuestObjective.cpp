// Copyright pjdevs. All Rights Reserved.


#include "Assets/ActionQuestObjective.h"

#include "Assets/ActionQuestEvent.h"

UActionQuestObjective::UActionQuestObjective()
	: ActionToDo(FName("None")), TimesToDoAction(1)
{
	bRetroCompletable = false;
}

int UActionQuestObjective::GetCompletion_Implementation(UWorld* World)
{
	return 0;
}

int UActionQuestObjective::GetTargetValue_Implementation()
{
	return TimesToDoAction;
}

int UActionQuestObjective::TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event)
{
	if (const UActionQuestEvent* ActionEvent = Cast<UActionQuestEvent>(Event))
	{
		if (ActionEvent->ActionName == ActionToDo)
		{
			if (const APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				if (PlayerController == ActionEvent->ActionInstigator)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}
