// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemId.h"
#include "Components/ActorComponent.h"
#include "InventoryNotificationsComponent.generated.h"


class UBaseNotificationWidget;

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class QUESTINTEGRATIONPLUGIN_API UInventoryNotificationsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryNotificationsComponent();

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnItemAdded(FInventoryItemId ItemId, int ItemCount);
	
	UFUNCTION()
	void OnItemRemoved(FInventoryItemId ItemId, int ItemCount);

	void SubmitNotification(FInventoryItemId ItemId, int ItemCount, bool bIsRemove) const;

	bool IsLocal() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = Notification, meta = (AllowPrivateAccess = true))
	FText ItemAddedTemplateText;

	UPROPERTY(EditDefaultsOnly, Category = Notification, meta = (AllowPrivateAccess = true))
	FText ItemRemovedTemplateText;

	UPROPERTY(EditDefaultsOnly, Category = Notification, meta = (AllowPrivateAccess = true))
	TSubclassOf<UBaseNotificationWidget> NotificationWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = Notification, meta = (AllowPrivateAccess = true))
	float NotificationDuration;
};
