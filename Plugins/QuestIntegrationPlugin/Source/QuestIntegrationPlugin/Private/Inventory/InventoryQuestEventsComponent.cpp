// Copyright pjdevs. All Rights Reserved.


#include "Inventory/InventoryQuestEventsComponent.h"
#include "InventoryComponent.h"
#include "InventoryStatics.h"
#include "QuestStatics.h"
#include "Quest/InventoryQuestEvent.h"


UInventoryQuestEventsComponent::UInventoryQuestEventsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UInventoryQuestEventsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	if (UInventoryComponent* InventoryComponent = GetOwner()->GetComponentByClass<UInventoryComponent>())
	{
		InventoryComponent->OnItemAddedDelegate.AddDynamic(this, &UInventoryQuestEventsComponent::OnItemAdded);
		InventoryComponent->OnItemRemovedDelegate.AddDynamic(this, &UInventoryQuestEventsComponent::OnItemRemoved);
	}
}

void UInventoryQuestEventsComponent::OnItemAdded(FInventoryItemId ItemId, int ItemCount)
{
	SubmitItemCountChangedQuestEvent(ItemId);
}

void UInventoryQuestEventsComponent::OnItemRemoved(FInventoryItemId ItemId, int ItemCount)
{
	SubmitItemCountChangedQuestEvent(ItemId);
}

void UInventoryQuestEventsComponent::SubmitItemCountChangedQuestEvent(FInventoryItemId ItemId) const
{
	const UInventoryComponent* SharedInventory = UInventoryStatics::GetSharedInventory(GetWorld());

	if (!SharedInventory)
	{
		return;
	}
	
	UInventoryQuestEvent* Event = NewObject<UInventoryQuestEvent>();
	Event->ItemId = ItemId;
	Event->NewItemCount = SharedInventory->GetItemCount(ItemId);

	UQuestStatics::SubmitQuestEvent(GetWorld(), Event);
}
