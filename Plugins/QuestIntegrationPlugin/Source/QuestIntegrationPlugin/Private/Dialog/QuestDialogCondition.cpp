// Copyright pjdevs. All Rights Reserved.


#include "Dialog/QuestDialogCondition.h"

#include "QuestComponent.h"
#include "QuestStatics.h"


bool UQuestDialogCondition::IsSatisfied_Implementation(AController* DialogController)
{
	if (const UQuestComponent* QuestSubsystem = UQuestStatics::GetQuestComponent(GetWorld()))
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
