#include "InventoryDialogCondition.h"
#include "QuestWorld/Inventory/InventoryHelpers.h"

bool UInventoryDialogCondition::IsSatisfied(UWorld* World)
{
	if (!World)
	{
		return false;
	}

	const int Total = FInventoryHelpers::GetTotalItemCountForAllPlayers(World, TargetItemName);

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
