// Copyright pjdevs. All Rights Reserved.


#include "Inventory/InventoryNotificationsComponent.h"

#include "GameNotification.h"
#include "InventoryComponent.h"
#include "InventoryItemDataAsset.h"
#include "InventoryStatics.h"
#include "NotificationSubsystem.h"


UInventoryNotificationsComponent::UInventoryNotificationsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UInventoryNotificationsComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!IsLocal())
	{
		return;
	}

	if (UInventoryComponent* InventoryComponent = GetOwner()->GetComponentByClass<UInventoryComponent>())
	{
		InventoryComponent->OnItemAddedDelegate.AddDynamic(this, &UInventoryNotificationsComponent::OnItemAdded);
		InventoryComponent->OnItemRemovedDelegate.AddDynamic(this, &UInventoryNotificationsComponent::OnItemRemoved);
	}
}

void UInventoryNotificationsComponent::OnItemAdded(FInventoryItemId ItemId, int ItemCount)
{
	SubmitNotification(ItemId, ItemCount, false);
}

void UInventoryNotificationsComponent::OnItemRemoved(FInventoryItemId ItemId, int ItemCount)
{
	SubmitNotification(ItemId, ItemCount, true);
}

void UInventoryNotificationsComponent::SubmitNotification(FInventoryItemId ItemId, int ItemCount, bool bIsRemove) const
{
	const UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	const UInventoryItemDataAsset* Item = UInventoryStatics::GetItem(ItemId);
	const FText& TemplateText = bIsRemove ? ItemRemovedTemplateText : ItemAddedTemplateText;
	const FText NotificationText = FText::Format(TemplateText, Item->GetItemName(), ItemCount);

	UNotificationSubsystem* NotificationSubsystem = World->GetGameInstance()->GetSubsystem<UNotificationSubsystem>();

	NotificationSubsystem->QueueNotification(FGameNotification
	{
		.NotificationWidgetClass = NotificationWidgetClass,
		.NotificationCategory = FName(TEXT("Inventory")),
		.Message = NotificationText,
		.Duration = NotificationDuration,
		.Icon = Item->GetItemIcon()
	});
}

bool UInventoryNotificationsComponent::IsLocal() const
{
	if (const APlayerController* OwnerController = Cast<APlayerController>(GetOwner()))
	{
		return OwnerController->IsLocalController();
	}

	if (const APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		return OwnerPawn->IsLocallyControlled();
	}

	return true;
}

