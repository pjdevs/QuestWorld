// Copyright pjdevs. All Rights Reserved.


#include "ItemInteractive.h"

#include "InventoryComponent.h"
#include "InventoryItemDataAsset.h"
#include "InventoryStatics.h"


FIPInteractionStatus AItemInteractive::GetInteractionStatusForActor_Implementation(
	AActor* InteractionInstigator,
	const FIPInteractiveState& CurrentState
) const
{
	FText ItemName;

	if (UInventoryComponent* SharedInventory = UInventoryStatics::GetSharedInventory(GetWorld()))
	{
		ItemName = UInventoryStatics::GetItem(RequiredItem)->GetItemName();
		
		if (SharedInventory->GetItemCount(RequiredItem) > 0)
		{
			return FIPInteractionStatus
			{
				.bCanStartInteraction = true
			};
		}
	}

	return FIPInteractionStatus
	{
		.bCanStartInteraction = false,
		.ReasonText = FText::Format(RequiredItemTextFormat, ItemName)
	};
}
