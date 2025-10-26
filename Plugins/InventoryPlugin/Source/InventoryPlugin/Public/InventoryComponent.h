// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemId.h"
#include "InventorySaveData.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UInventoryItemDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryChangedDelegate, FInventoryItemId, ItemId, int, ItemCount);

class UInventoryComponent;

USTRUCT(BlueprintType)
struct INVENTORYPLUGIN_API FInventoryItemEntry: public FFastArraySerializerItem
{
	GENERATED_BODY()

	bool operator==(const FInventoryItemId& OtherItemId) const
	{
		return ItemId == OtherItemId;
	}

	UPROPERTY()
	FInventoryItemId ItemId = FInventoryItemId();

	UPROPERTY()
	int Quantity = 0;

	int LastQuantity = 0;
};

USTRUCT()
struct INVENTORYPLUGIN_API FInventoryList : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FInventoryList();
	explicit FInventoryList(UInventoryComponent* InventoryComponent);

	void AddItem(const FInventoryItemId& ItemId, int ItemCountToAdd);
	void RemoveItem(const FInventoryItemId& ItemId, int ItemCountToRemove);
	int GetItemCount(const FInventoryItemId& ItemId) const;
	const TArray<FInventoryItemEntry>& GetItems() const { return Items; }

public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);

private:
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

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void AddItem(FInventoryItemId ItemId, int ItemCountToAdd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void RemoveItem(FInventoryItemId ItemId, int ItemCountToRemove);
	
	UFUNCTION(BlueprintPure, Category = Inventory)
	int GetItemCount(FInventoryItemId ItemId) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void LoadItemsFromSave(const FInventorySaveData& InventorySaveData);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	FInventorySaveData WriteItemsToSave();

public:
	UFUNCTION()
	void OnItemAddedClient(const FInventoryItemId& ItemId, int ItemCountAdded);

	UFUNCTION()
	void OnItemRemovedClient(const FInventoryItemId& ItemId, int ItemCountRemoved);

	UFUNCTION()
	void OnRep_InventoryList();

private:
	UPROPERTY(BlueprintAssignable, Category = Inventory, meta = (AllowPrivateAccess = true))
	FInventoryChangedDelegate OnItemAddedDelegate;

	UPROPERTY(BlueprintAssignable, Category = Inventory, meta = (AllowPrivateAccess = true))
	FInventoryChangedDelegate OnItemRemovedDelegate;
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_InventoryList)
	FInventoryList InventoryList;
	
	bool bInventoryReceived;
};
