// Copyright pjdevs. All Rights Reserved.


#include "Dialog/InventoryTrigger.h"

#include "InventoryComponent.h"
#include "InventoryStatics.h"

void UInventoryTrigger::OnExecute_Implementation(AController* DialogController)
{
	UInventoryComponent* Inventory;

	if (bShouldUseSharedInventory)
	{
		Inventory = UInventoryStatics::GetSharedInventory(GetWorld());		
	}
	else
	{
		Inventory = DialogController->GetPawn()->GetComponentByClass<UInventoryComponent>();
	}

	if (!Inventory)
	{
		return;
	}

	if (bShouldRemove)
	{
		Inventory->RemoveItem(ItemId, ItemCount);
	}
	else
	{
		Inventory->AddItem(ItemId, ItemCount);
	}
}
