// Copyright pjdevs. All Rights Reserved.

#include "InventoryComponent.h"

#include "InventoryStatics.h"
#include "Net/UnrealNetwork.h"


FInventoryList::FInventoryList()
	: OwnerComponent(nullptr)
{
}

FInventoryList::FInventoryList(UInventoryComponent* InventoryComponent)
	: OwnerComponent(InventoryComponent)
{
}

void FInventoryList::AddItem(const FInventoryItemId& ItemId, int ItemCountToAdd)
{
	FInventoryItemEntry* Existing = Items.FindByPredicate(
	[&](const FInventoryItemEntry& Item) { return Item.ItemId == ItemId; }
	);

	if (Existing)
	{
		Existing->Quantity += ItemCountToAdd;
		MarkItemDirty(*Existing);
	}
	else
	{
		FInventoryItemEntry& NewItem = Items.AddDefaulted_GetRef();
		NewItem.ItemId = ItemId;
		NewItem.Quantity = ItemCountToAdd;
		MarkItemDirty(NewItem);
	}
}

void FInventoryList::RemoveItem(const FInventoryItemId& ItemId, int ItemCountToRemove)
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i].ItemId == ItemId)
		{
			Items[i].Quantity -= ItemCountToRemove;

			if (Items[i].Quantity <= 0)
			{
				MarkItemDirty(Items[i]);
				Items.RemoveAt(i);
			}
			else
			{
				MarkItemDirty(Items[i]);
			}
			break;
		}
	}
}

int FInventoryList::GetItemCount(const FInventoryItemId& ItemId) const
{
	const FInventoryItemEntry* Entry = Items.FindByKey(ItemId);
	return Entry ? Entry->Quantity : 0;
}

bool FInventoryList::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FastArrayDeltaSerialize<FInventoryItemEntry, FInventoryList>(Items, DeltaParms, *this);
}

void FInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	if (!IsValid(OwnerComponent))
	{
		return;
	}

	for (const int32 Idx : AddedIndices)
	{
		OwnerComponent->OnItemAdded(Items[Idx]);
	}
}

void FInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	if (!IsValid(OwnerComponent))
	{
		return;
	}

	for (const int32 Idx : ChangedIndices)
	{
		OwnerComponent->OnItemChanged(Items[Idx]);
	}
}

void FInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	if (!IsValid(OwnerComponent))
	{
		return;
	}

	for (const int32 Idx : RemovedIndices)
	{
		OwnerComponent->OnItemRemoved(Items[Idx].ItemId);
	}
}

UInventoryComponent::UInventoryComponent()
	: InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryComponent, InventoryList);
}

void UInventoryComponent::AddItem(FInventoryItemId ItemId, int ItemCountToAdd)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	if (!UInventoryStatics::IsItemExisting(ItemId))
	{
		return;
	}

	if (ItemCountToAdd <= 0)
	{
		return;
	}

	InventoryList.AddItem(ItemId, ItemCountToAdd);
	OnItemCountChanged.Broadcast(ItemId, GetItemCount(ItemId));
}

void UInventoryComponent::RemoveItem(FInventoryItemId ItemId, int ItemCountToRemove)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	if (!UInventoryStatics::IsItemExisting(ItemId))
	{
		return;
	}

	if (ItemCountToRemove <= 0)
	{
		return;
	}

	InventoryList.RemoveItem(ItemId, ItemCountToRemove);
	OnItemCountChanged.Broadcast(ItemId, GetItemCount(ItemId));
}

int UInventoryComponent::GetItemCount(FInventoryItemId ItemId) const
{
	const FInventoryItemEntry* Entry = InventoryList.Items.FindByKey(ItemId);
	return Entry ? Entry->Quantity : 0;
}

void UInventoryComponent::OnItemAdded(const FInventoryItemEntry& ItemEntry)
{
	OnItemCountChanged.Broadcast(ItemEntry.ItemId, ItemEntry.Quantity);
}

void UInventoryComponent::OnItemChanged(const FInventoryItemEntry& ItemEntry)
{
	OnItemCountChanged.Broadcast(ItemEntry.ItemId, ItemEntry.Quantity);
}

void UInventoryComponent::OnItemRemoved(const FInventoryItemId& ItemId)
{
	OnItemCountChanged.Broadcast(ItemId, 0);
}
