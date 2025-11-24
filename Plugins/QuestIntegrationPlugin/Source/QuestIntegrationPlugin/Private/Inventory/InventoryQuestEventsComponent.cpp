// Copyright pjdevs. All Rights Reserved.


#include "Inventory/InventoryQuestEventsComponent.h"
#include "InventoryComponent.h"
#include "InventoryStatics.h"
#include "QuestStatics.h"
#include "QuestSubsystem.h"
#include "Quest/InventoryQuestEvent.h"


UInventoryQuestEventsComponent::UInventoryQuestEventsComponent()
	: OwnerInventory(nullptr)
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

	OwnerInventory = GetOwner()->GetComponentByClass<UInventoryComponent>();

	if (!OwnerInventory)
	{
		return;
	}

	OwnerInventory->OnItemAddedDelegate.AddDynamic(this, &UInventoryQuestEventsComponent::OnItemAdded);
	OwnerInventory->OnItemRemovedDelegate.AddDynamic(this, &UInventoryQuestEventsComponent::OnItemRemoved);
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
	if (!OwnerInventory)
	{
		return;
	}
	
	UInventoryQuestEvent* Event = NewObject<UInventoryQuestEvent>();
	Event->ItemId = ItemId;
	Event->NewItemCount = OwnerInventory->GetItemCount(ItemId);

	if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
	{
		QuestSubsystem->SubmitQuestEvent(Event);
	}
}
