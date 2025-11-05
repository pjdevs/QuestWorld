// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemId.h"
#include "Components/ActorComponent.h"
#include "InventoryQuestEventsComponent.generated.h"


class UInventoryComponent;

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class QUESTINTEGRATIONPLUGIN_API UInventoryQuestEventsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryQuestEventsComponent();

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnItemAdded(FInventoryItemId ItemId, int ItemCount);
	
	UFUNCTION()
	void OnItemRemoved(FInventoryItemId ItemId, int ItemCount);

	void SubmitItemCountChangedQuestEvent(FInventoryItemId ItemId) const;

private:
	UInventoryComponent* OwnerInventory;
};
