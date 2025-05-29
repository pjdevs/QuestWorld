// Copyright pjdevs. All Rights Reserved.


#include "Assets/SimpleQuestObjective.h"

bool USimpleQuestObjective::IsSatisfied_Implementation(UWorld* World)
{
	return false; // UGameplayS ->Tags.Contains(TagToCheck);
}
