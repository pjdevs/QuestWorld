// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemId.h"
#include "ISpudObject.h"
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

	UPROPERTY(SaveGame)
	FInventoryItemId ItemId = FInventoryItemId();

	UPROPERTY(SaveGame)
	int Quantity = 0;

	UPROPERTY()
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
	TArray<FInventoryItemEntry>& GetItems() { return Items; }

public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);

private:
	TWeakObjectPtr<UInventoryComponent> OwnerComponent;
	
	UPROPERTY(SaveGame)
	TArray<FInventoryItemEntry> Items;
};

template<>
struct TStructOpsTypeTraits<FInventoryList> : public TStructOpsTypeTraitsBase2<FInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};

UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class INVENTORYPLUGIN_API UInventoryComponent : public UActorComponent, public ISpudObject, public ISpudObjectCallback
{
	GENERATED_BODY()

public:
	UInventoryComponent();

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public: // Spud
	virtual void SpudPostRestore_Implementation(const USpudState* SpudState) override;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void AddItem(FInventoryItemId ItemId, int ItemCountToAdd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Inventory)
	void RemoveItem(FInventoryItemId ItemId, int ItemCountToRemove);
	
	UFUNCTION(BlueprintPure, Category = Inventory)
	int GetItemCount(FInventoryItemId ItemId) const;

public:
	UFUNCTION()
	void OnItemAddedClient(const FInventoryItemId& ItemId, int ItemCountAdded);

	UFUNCTION()
	void OnItemRemovedClient(const FInventoryItemId& ItemId, int ItemCountRemoved);

	UFUNCTION()
	void OnRep_InventoryList();

public:
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FInventoryChangedDelegate OnItemAddedDelegate;

	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FInventoryChangedDelegate OnItemRemovedDelegate;
	
private:
	UPROPERTY(SaveGame, ReplicatedUsing=OnRep_InventoryList)
	FInventoryList InventoryList;
	
	bool bInventoryReceived;
};
