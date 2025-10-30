// Copyright pjdevs. All Rights Reserved.


#include "Dialog/StartQuestDialogTrigger.h"
#include "QuestComponent.h"
#include "QuestStatics.h"


void UStartQuestDialogTrigger::OnExecute_Implementation(AController* DialogController)
{
	if (UQuestComponent* QuestSubsystem = UQuestStatics::GetQuestComponent(GetWorld()))
	{
		QuestSubsystem->StartQuest(QuestToStartId);
	}	
}
