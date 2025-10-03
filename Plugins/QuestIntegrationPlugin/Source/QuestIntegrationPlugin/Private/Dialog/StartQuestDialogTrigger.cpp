// Copyright pjdevs. All Rights Reserved.


#include "Dialog/StartQuestDialogTrigger.h"

#include "QuestSubsystem.h"

void UStartQuestDialogTrigger::Execute(UWorld* World, AController* DialogController)
{
	if (UQuestSubsystem* QuestSubsystem = UQuestSubsystem::GetFromWorld(World))
	{
		QuestSubsystem->StartQuest(QuestToStartId);
	}		
}
