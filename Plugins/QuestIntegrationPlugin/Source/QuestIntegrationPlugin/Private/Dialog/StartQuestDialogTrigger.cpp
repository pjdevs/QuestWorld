// Copyright pjdevs. All Rights Reserved.


#include "Dialog/StartQuestDialogTrigger.h"

#include "QuestSubsystem.h"

void UStartQuestDialogTrigger::Execute_Implementation()
{
	if (UQuestSubsystem* QuestSubsystem = UQuestSubsystem::GetFromWorld(GetWorld()))
	{
		QuestSubsystem->StartQuest(QuestToStartId);
	}		
}
