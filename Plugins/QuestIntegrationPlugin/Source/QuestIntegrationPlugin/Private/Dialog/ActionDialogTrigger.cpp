// Copyright pjdevs. All Rights Reserved.


#include "Dialog/ActionDialogTrigger.h"

#include "QuestSubsystem.h"
#include "Assets/ActionQuestEvent.h"

void UActionDialogTrigger::Execute(UWorld* World, AController* DialogController)
{
	if (!World)
	{
		return;
	}
	
	if (UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
	{
		UActionQuestEvent* ActionQuestEvent = NewObject<UActionQuestEvent>();
		ActionQuestEvent->ActionInstigator = DialogController;
		ActionQuestEvent->ActionName = ActionToTrigger;
		QuestSubsystem->SubmitQuestEvent(ActionQuestEvent);
	}
}
