// Copyright pjdevs. All Rights Reserved.

#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddItem(const FString& ItemName, int ItemCount)
{
	if (InventoryMap.Contains(ItemName))
	{
		int* CurrentItemCount = InventoryMap.Find(ItemName);
		*CurrentItemCount += ItemCount;
	}
	else
	{
		InventoryMap.Add(ItemName, ItemCount);
	}

	InventoryItemsAdded.Broadcast(ItemName, ItemCount);
}

int UInventoryComponent::GetItemCount(const FString& ItemName) const
{
	return InventoryMap.Contains(ItemName) ? *InventoryMap.Find(ItemName) : 0;
}
