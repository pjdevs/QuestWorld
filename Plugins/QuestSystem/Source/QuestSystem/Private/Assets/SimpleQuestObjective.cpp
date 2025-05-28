// Copyright pjdevs. All Rights Reserved.


#include "Assets/SimpleQuestObjective.h"

bool USimpleQuestObjective::IsSatisfied_Implementation(UWorld* World, APlayerController* Player)
{
	return Player->Tags.Contains(TagToCheck);
}
