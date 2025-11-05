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
	FText RequiredItemName;

	if (UInventoryComponent* SharedInventory = UInventoryStatics::GetSharedInventory(GetWorld()))
	{
		RequiredItemName = UInventoryStatics::GetItem(RequiredItem)->GetItemName();

		if (SharedInventory->GetItemCount(RequiredItem) > 0)
		{
			return FIPInteractionStatus
			{
				.bCanStartInteraction = true
			};
		}
	}

	if (const UInventoryComponent* Inventory = InteractionInstigator->GetComponentByClass<UInventoryComponent>())
	{
		for (const FInventoryItemId& AdditionalItem : BonusHiddenItems)
		{
			if (Inventory->GetItemCount(AdditionalItem) > 0)
			{
				return FIPInteractionStatus
				{
					.bCanStartInteraction = true
				};
			}
		}
	}

	return FIPInteractionStatus
	{
		.bCanStartInteraction = false,
		.ReasonText = FText::Format(RequiredItemTextFormat, RequiredItemName)
	};
}
