#include "InventoryDialogCondition.h"
#include "InventoryStatics.h"

bool UInventoryDialogCondition::IsSatisfied(UWorld* World)
{
	if (!World)
	{
		return false;
	}

	const int Total = UInventoryStatics::GetTotalItemCountForAllPlayers(World, TargetItemId);

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
