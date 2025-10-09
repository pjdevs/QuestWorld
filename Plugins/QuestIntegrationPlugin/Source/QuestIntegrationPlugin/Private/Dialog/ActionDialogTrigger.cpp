// Copyright pjdevs. All Rights Reserved.


#include "Dialog/ActionDialogTrigger.h"

#include "QuestComponent.h"
#include "QuestStatics.h"
#include "Assets/ActionQuestEvent.h"

void UActionDialogTrigger::Execute(UWorld* World, AController* DialogController)
{
	if (!World)
	{
		return;
	}
	
	if (UQuestComponent* QuestSubsystem = UQuestStatics::GetQuestComponent(World))
	{
		UActionQuestEvent* ActionQuestEvent = NewObject<UActionQuestEvent>();
		ActionQuestEvent->ActionInstigator = DialogController;
		ActionQuestEvent->ActionName = ActionToTrigger;
		QuestSubsystem->SubmitQuestEvent(ActionQuestEvent);
	}
}
