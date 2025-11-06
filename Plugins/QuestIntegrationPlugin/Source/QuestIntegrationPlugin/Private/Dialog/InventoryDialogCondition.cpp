#include "Dialog/InventoryDialogCondition.h"

#include "InventoryComponent.h"
#include "InventoryStatics.h"

bool UInventoryDialogCondition::IsSatisfied_Implementation(AController* DialogController)
{
	int Total = 0;

	if (bShouldUseSharedInventory)
	{
		if (const UInventoryComponent* SharedInventory = UInventoryStatics::GetSharedInventory(GetWorld()))
		{
			Total = SharedInventory->GetItemCount(TargetItemId);
		}
	}
	else
	{
		if (const UInventoryComponent* PlayerInventory = UInventoryStatics::GetPlayerInventory(DialogController))
		{
			Total = PlayerInventory->GetItemCount(TargetItemId);
		}
	}

	switch (Behavior)
	{
	case EInventoryConditionBehavior::More:
		return Total >= TargetItemCount;
	case EInventoryConditionBehavior::Less:
		return Total <= TargetItemCount;
	default:
		return false;
	}
}
