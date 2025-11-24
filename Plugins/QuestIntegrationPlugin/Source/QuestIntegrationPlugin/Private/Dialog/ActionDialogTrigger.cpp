// Copyright pjdevs. All Rights Reserved.


#include "Dialog/ActionDialogTrigger.h"
#include "QuestSubsystem.h"
#include "Assets/ActionQuestEvent.h"

void UActionDialogTrigger::OnExecute_Implementation(AController* DialogController)
{
	if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
	{
		UActionQuestEvent* ActionQuestEvent = NewObject<UActionQuestEvent>();
		ActionQuestEvent->ActionInstigator = DialogController;
		ActionQuestEvent->ActionName = ActionToTrigger;

		QuestSubsystem->SubmitQuestEvent(ActionQuestEvent);
	}
}
