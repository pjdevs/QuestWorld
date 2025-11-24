// Copyright pjdevs. All Rights Reserved.


#include "Dialog/StartQuestDialogTrigger.h"
#include "QuestSubsystem.h"


void UStartQuestDialogTrigger::OnExecute_Implementation(AController* DialogController)
{
	if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
	{
		QuestSubsystem->StartQuest(QuestToStartId);
	}	
}
