// Copyright pjdevs. All Rights Reserved.


#include "InventoryObjective.h"

#include "InventoryQuestEvent.h"
#include "QuestWorld/Inventory/InventoryHelpers.h"

UInventoryObjective::UInventoryObjective()
{
	bRetroCompletable = true;
}

int UInventoryObjective::GetCompletion_Implementation(UWorld* World)
{
	if (!World)
	{
		return 0;
	}
	
	return FInventoryHelpers::GetTotalItemCountForAllPlayers(World, TargetItemName);
}

int UInventoryObjective::GetTargetValue_Implementation()
{
	return TargetItemCount;
}

int UInventoryObjective::TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event)
{
	if (const UInventoryQuestEvent* InventoryEvent = Cast<UInventoryQuestEvent>(Event))
	{
		if (InventoryEvent->NameOfItemAdded == TargetItemName)
		{
			return TargetItemCount;
		}
	}

	return 0;
}
