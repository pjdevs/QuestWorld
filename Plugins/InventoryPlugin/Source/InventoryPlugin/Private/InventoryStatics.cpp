// Copyright pjdevs. All Rights Reserved.


#include "InventoryStatics.h"

#include "InventoryComponent.h"
#include "InventoryItemDataAsset.h"
#include "Engine/AssetManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

UInventoryItemDataAsset* UInventoryStatics::GetItem(FInventoryItemId ItemId)
{
	const UAssetManager& AssetManager = UAssetManager::Get();
	const FSoftObjectPath ItemAssetPath = AssetManager.GetPrimaryAssetPath(ItemId.InventoryItemAssetId);

	if (!ItemAssetPath.IsValid())
	{
		return nullptr;
	}

	const TSoftObjectPtr<UInventoryItemDataAsset> SoftItemPtr(ItemAssetPath);
	UInventoryItemDataAsset* ItemAsset = SoftItemPtr.LoadSynchronous();

	if (!IsValid(ItemAsset))
	{
		return nullptr;
	}
	
	return ItemAsset;
}

bool UInventoryStatics::IsItemExisting(FInventoryItemId ItemId)
{
	if (!ItemId.IsValid())
	{
		return false;
	}

 	const UAssetManager& AssetManager = UAssetManager::Get();
	const FSoftObjectPath ItemAssetPath = AssetManager.GetPrimaryAssetPath(ItemId.InventoryItemAssetId);

	return ItemAssetPath.IsValid();
}

int UInventoryStatics::GetTotalItemCountForAllPlayers(const UWorld* World, FInventoryItemId ItemId)
{
	int TotalItemCount = 0; 
	
	for (
		FConstPlayerControllerIterator PlayerControllerIt = World->GetPlayerControllerIterator();
		PlayerControllerIt;
		++PlayerControllerIt
	)
	{
		if (const UInventoryComponent* Inventory = GetPlayerInventory(PlayerControllerIt->Get()))
		{
			TotalItemCount += Inventory->GetItemCount(ItemId);
		}
	}

	return TotalItemCount;
}

UInventoryComponent* UInventoryStatics::GetSharedInventory(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (UInventoryComponent* SharedInventory = World->GetGameState()->FindComponentByClass<UInventoryComponent>())
	{
		return SharedInventory;
	}

	return nullptr;
}

UInventoryComponent* UInventoryStatics::GetPlayerInventory(const AActor* Actor)
{
	const APlayerState* PlayerState = nullptr;
	
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		PlayerState = Pawn->GetPlayerState();
	}

	if (const AController* Controller = Cast<AController>(Actor))
	{
		PlayerState = Controller->PlayerState;
	}

	if (PlayerState)
	{
		return PlayerState->GetComponentByClass<UInventoryComponent>();
	}

	return nullptr;
}
