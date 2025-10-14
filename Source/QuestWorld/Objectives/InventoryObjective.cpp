// Copyright pjdevs. All Rights Reserved.


#include "InventoryObjective.h"

#include "InventoryQuestEvent.h"
#include "InventoryStatics.h"

UInventoryObjective::UInventoryObjective()
{
	bRetroCompletable = true;
	bAddProgress = false;
}

int UInventoryObjective::GetCompletion_Implementation(UWorld* World)
{
	if (!World)
	{
		return 0;
	}
	
	return UInventoryStatics::GetTotalItemCountForAllPlayers(World, TargetItemId);
}

int UInventoryObjective::GetTargetValue_Implementation()
{
	return TargetItemCount;
}

int UInventoryObjective::TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event)
{
	if (const UInventoryQuestEvent* InventoryEvent = Cast<UInventoryQuestEvent>(Event))
	{
		if (InventoryEvent->ItemId == TargetItemId)
		{
			return GetCompletion_Implementation(World);
		}
	}

	return 0;
}
