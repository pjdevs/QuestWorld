// Copyright pjdevs. All Rights Reserved.


#include "CollectableItem.h"
#include "InventoryComponent.h"
#include "InventoryStatics.h"


ACollectableItem::ACollectableItem()
{
	bIsSavable = true;
}

void ACollectableItem::OnStartInteractionInput_Implementation(AActor* InteractionInstigator)
{
	if (UInventoryComponent* SharedInventory = UInventoryStatics::GetSharedInventory(GetWorld()))
	{
		SharedInventory->AddItem(GivesItem, 1);
	}

	EndInteractionPhase(EIPInteractiveState::Destroyed);
}
