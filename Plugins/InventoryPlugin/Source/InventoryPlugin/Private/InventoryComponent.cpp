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
		Existing->LastQuantity = Existing->Quantity;
		Existing->Quantity += ItemCountToAdd;
		MarkItemDirty(*Existing);
	}
	else
	{
		FInventoryItemEntry& NewItem = Items.AddDefaulted_GetRef();
		NewItem.ItemId = ItemId;
		NewItem.LastQuantity = 0;
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
			Items[i].LastQuantity = Items[i].Quantity;
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
		FInventoryItemEntry Entry = Items[Idx];
		OwnerComponent->OnItemAddedClient(Entry.ItemId, Entry.Quantity);
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
		const FInventoryItemEntry& Entry = Items[Idx];
		const int DeltaQuantity = Entry.Quantity - Entry.LastQuantity;

		if (DeltaQuantity > 0)
		{
			OwnerComponent->OnItemAddedClient(Entry.ItemId, DeltaQuantity);
		}
		else
		{
			OwnerComponent->OnItemRemovedClient(Entry.ItemId, -DeltaQuantity);
		}
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
		FInventoryItemEntry Entry = Items[Idx];
		OwnerComponent->OnItemRemovedClient(Entry.ItemId, Entry.Quantity);
	}
}

UInventoryComponent::UInventoryComponent()
	: InventoryList(this), bInventoryReceived(false)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initial replication
	// TODO load saved inventory

	if (GetOwnerRole() == ROLE_Authority)
	{
		InventoryList.MarkArrayDirty();
		OnRep_InventoryList();
	}
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
	OnItemAddedDelegate.Broadcast(ItemId, ItemCountToAdd);
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

	const int ItemCount = InventoryList.GetItemCount(ItemId);
	const int RealCountToRemove = FMath::Min(ItemCountToRemove, ItemCount);

	InventoryList.RemoveItem(ItemId, RealCountToRemove);
	OnItemRemovedDelegate.Broadcast(ItemId, RealCountToRemove);
}

int UInventoryComponent::GetItemCount(FInventoryItemId ItemId) const
{
	return InventoryList.GetItemCount(ItemId);
}

void UInventoryComponent::LoadItemsFromSave(const FInventorySaveData& InventorySaveData)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}
	
	for (const FInventoryEntrySaveData& Item : InventorySaveData.Items)
	{
		InventoryList.AddItem(Item.ItemId, Item.Quantity);
	}

	OnRep_InventoryList();
}

FInventorySaveData UInventoryComponent::WriteItemsToSave()
{
	FInventorySaveData InventorySaveData;

	if (GetOwnerRole() != ROLE_Authority)
	{
		return InventorySaveData;
	}
	
	for (const auto& Entry : InventoryList.GetItems())
	{
		InventorySaveData.Items.Add(
			FInventoryEntrySaveData
			{
				.ItemId = Entry.ItemId,
				.Quantity = Entry.Quantity
			}
		);
	}

	return InventorySaveData;
}

void UInventoryComponent::OnItemAddedClient(const FInventoryItemId& ItemId, int ItemCountAdded)
{
	if (bInventoryReceived)
	{
		OnItemAddedDelegate.Broadcast(ItemId, ItemCountAdded);
	}
}

void UInventoryComponent::OnItemRemovedClient(const FInventoryItemId& ItemId, int ItemCountRemoved)
{
	if (bInventoryReceived)
	{
		OnItemAddedDelegate.Broadcast(ItemId, ItemCountRemoved);
	}
}

void UInventoryComponent::OnRep_InventoryList()
{
	bInventoryReceived = true;
}
