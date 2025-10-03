// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryItemsAdded, FString, ItemName, int, ItemCount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class QUESTWORLD_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	UFUNCTION(BlueprintCallable)
	void AddItem(const FString& ItemName, int ItemCount);

	UFUNCTION(BlueprintCallable)
	int GetItemCount(const FString& ItemName) const;

private:
	UPROPERTY(BlueprintAssignable, Category = Inventory, meta = (AllowPrivateAccess = true))
	FInventoryItemsAdded InventoryItemsAdded;

private:
	TMap<FString, int> InventoryMap;
};
