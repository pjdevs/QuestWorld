// Copyright pjdevs. All Rights Reserved.


#include "Dialog/ActionDialogTrigger.h"

#include "QuestComponent.h"
#include "QuestStatics.h"
#include "Assets/ActionQuestEvent.h"

void UActionDialogTrigger::OnExecute_Implementation(AController* DialogController)
{
	if (UQuestComponent* QuestSubsystem = UQuestStatics::GetQuestComponent(GetWorld()))
	{
		UActionQuestEvent* ActionQuestEvent = NewObject<UActionQuestEvent>();
		ActionQuestEvent->ActionInstigator = DialogController;
		ActionQuestEvent->ActionName = ActionToTrigger;
		QuestSubsystem->SubmitQuestEvent(ActionQuestEvent);
	}
}
