// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryChangedDelegate, FPrimaryAssetId, ItemId, int, ItemCount);

class UInventoryComponent;

USTRUCT(BlueprintType)
struct INVENTORYPLUGIN_API FInventoryItemEntry: public FFastArraySerializerItem
{
	GENERATED_BODY()

	bool operator==(const FPrimaryAssetId& OtherItemId) const
	{
		return ItemId == OtherItemId;
	}

	UPROPERTY()
	FPrimaryAssetId ItemId = FPrimaryAssetId();

	UPROPERTY()
	int Quantity = 0;
};

USTRUCT()
struct INVENTORYPLUGIN_API FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

	FInventoryList();
	explicit FInventoryList(UInventoryComponent* InventoryComponent);

	void AddItem(const FPrimaryAssetId& ItemId, int ItemCountToAdd);
	void RemoveItem(const FPrimaryAssetId& ItemId, int ItemCountToRemove);
	int GetItemCount(const FPrimaryAssetId& ItemId) const;
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);

	UPROPERTY()
	TObjectPtr<UInventoryComponent> OwnerComponent;
	
	UPROPERTY()
	TArray<FInventoryItemEntry> Items;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class INVENTORYPLUGIN_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void AddItem(FPrimaryAssetId ItemId, int ItemCountToAdd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void RemoveItem(FPrimaryAssetId ItemId, int ItemCountToRemove);
	
	UFUNCTION(BlueprintPure, Category = Inventory)
	int GetItemCount(FPrimaryAssetId ItemId) const;

public:
	UFUNCTION()
	void OnItemAdded(const FInventoryItemEntry& ItemEntry);

	UFUNCTION()
	void OnItemChanged(const FInventoryItemEntry& ItemEntry);

	UFUNCTION()
	void OnItemRemoved(const FPrimaryAssetId& ItemId);

private:
	UPROPERTY(BlueprintAssignable, Category = Inventory, meta = (AllowPrivateAccess = true))
	FInventoryChangedDelegate OnItemCountChanged;
	
private:
	UPROPERTY(Replicated)
	FInventoryList InventoryList;
};
