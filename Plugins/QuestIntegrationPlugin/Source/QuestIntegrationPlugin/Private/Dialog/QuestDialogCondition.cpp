// Copyright pjdevs. All Rights Reserved.


#include "Dialog/QuestDialogCondition.h"
#include "QuestSubsystem.h"


bool UQuestDialogCondition::IsSatisfied_Implementation(AController* DialogController)
{
	if (const UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
	{
		switch (Behavior)
		{
		case EQuestConditionBehavior::QuestNotStartNorCompleted:
			return !QuestSubsystem->IsQuestCompleted(QuestId) && !QuestSubsystem->IsQuestActive(QuestId);
		case EQuestConditionBehavior::QuestStarted:
			return QuestSubsystem->IsQuestActive(QuestId);
		case EQuestConditionBehavior::QuestCompleted:
			return QuestSubsystem->IsQuestCompleted(QuestId);
		}
	}

	return false;
}
