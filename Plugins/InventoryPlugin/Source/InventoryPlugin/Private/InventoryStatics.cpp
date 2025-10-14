// Copyright pjdevs. All Rights Reserved.


#include "InventoryStatics.h"

#include "InventoryComponent.h"
#include "InventoryItemDataAsset.h"
#include "PrimaryAssetTypes.h"
#include "Engine/AssetManager.h"

UInventoryItemDataAsset* UInventoryStatics::GetItem(FPrimaryAssetId ItemId)
{
	const UAssetManager& AssetManager = UAssetManager::Get();
	const FSoftObjectPath ItemAssetPath = AssetManager.GetPrimaryAssetPath(ItemId);

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

bool UInventoryStatics::IsItemExisting(FPrimaryAssetId ItemId)
{
	if (!ItemId.IsValid() || ItemId.PrimaryAssetType != PrimaryAssetTypes::InventoryItem)
	{
		return false;
	}

 	const UAssetManager& AssetManager = UAssetManager::Get();
	const FSoftObjectPath ItemAssetPath = AssetManager.GetPrimaryAssetPath(ItemId);

	return ItemAssetPath.IsValid();
}

int UInventoryStatics::GetTotalItemCountForAllPlayers(const UWorld* World, FPrimaryAssetId ItemId)
{
	int TotalItemCount = 0; 
	
	for (
		FConstPlayerControllerIterator PlayerControllerIt = World->GetPlayerControllerIterator();
		PlayerControllerIt;
		++PlayerControllerIt
	)
	{
		const TWeakObjectPtr<APlayerController> PlayerController = *PlayerControllerIt;
		
		if (const TObjectPtr<APawn> Pawn = PlayerController->GetPawn())
		{
			if (const UInventoryComponent* Inventory = Pawn->GetComponentByClass<UInventoryComponent>())
			{
				TotalItemCount += Inventory->GetItemCount(ItemId);
			}
		}
	}

	return TotalItemCount;
}
