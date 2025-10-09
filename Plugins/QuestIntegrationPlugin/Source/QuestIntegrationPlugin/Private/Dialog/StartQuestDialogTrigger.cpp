// Copyright pjdevs. All Rights Reserved.


#include "Dialog/StartQuestDialogTrigger.h"

#include "QuestComponent.h"
#include "QuestStatics.h"


void UStartQuestDialogTrigger::Execute(UWorld* World, AController* DialogController)
{
	if (UQuestComponent* QuestSubsystem = UQuestStatics::GetQuestComponent(World))
	{
		QuestSubsystem->StartQuest(QuestToStartId);
	}		
}
