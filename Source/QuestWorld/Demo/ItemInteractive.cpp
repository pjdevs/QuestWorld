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

	if (const UInventoryComponent* SharedInventory = UInventoryStatics::GetSharedInventory(GetWorld()))
	{
		const UInventoryItemDataAsset* RequiredItemAsset = UInventoryStatics::GetItem(RequiredItem);
		
		if (RequiredItemAsset)
		{
			RequiredItemName = RequiredItemAsset->GetItemName();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not load item %s."), *RequiredItem.InventoryItemAssetId.ToString());
		}

		if (SharedInventory->GetItemCount(RequiredItem) > 0)
		{
			return FIPInteractionStatus
			{
				.bCanStartInteraction = true
			};
		}
	}
	
	if (const UInventoryComponent* Inventory = UInventoryStatics::GetPlayerInventory(InteractionInstigator))
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
