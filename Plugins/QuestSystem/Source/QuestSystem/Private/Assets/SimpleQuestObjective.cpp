// Copyright pjdevs. All Rights Reserved.


#include "Assets/SimpleQuestObjective.h"
#include "Assets/SimpleQuestEvent.h"

USimpleQuestObjective::USimpleQuestObjective()
{
	bRetroCompletable = true;
}

int USimpleQuestObjective::GetCompletion_Implementation(UWorld* World)
{
	if (const APlayerController* PlayerController = World->GetFirstPlayerController())
	{
		if (PlayerController->Tags.Contains(TagToCheck))
		{
			return 1;
		}
	}

	return 0;
}

int USimpleQuestObjective::GetTargetValue_Implementation()
{
	return 1;
}

int USimpleQuestObjective::TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event)
{
	if (const USimpleQuestEvent* SimpleEvent = Cast<USimpleQuestEvent>(Event))
	{
		if (SimpleEvent->TagAdded == TagToCheck)
		{
			if (const APlayerController* PlayerController = World->GetFirstPlayerController())
			{
				if (PlayerController == SimpleEvent->AffectedPlayerController)
				{
					return 1;
				}
			}
		}
	}

	return 0;
}
