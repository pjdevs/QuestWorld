// Copyright pjdevs. All Rights Reserved.


#include "Quest/InventoryObjective.h"

#include "InventoryComponent.h"
#include "Quest/InventoryQuestEvent.h"
#include "InventoryStatics.h"

UInventoryObjective::UInventoryObjective()
{
	bIsRetroCompletable = true;
	bShouldAddProgress = false;
}

int UInventoryObjective::GetCompletion_Implementation(UWorld* World) const
{
	if (!World)
	{
		return 0;
	}
	
	return UInventoryStatics::GetSharedInventory(World)->GetItemCount(TargetItemId);
}

int UInventoryObjective::GetTargetValue_Implementation() const
{
	return TargetItemCount;
}

int UInventoryObjective::TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event) const
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
