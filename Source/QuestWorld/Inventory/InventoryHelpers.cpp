// Copyright pjdevs. All Rights Reserved.


#include "InventoryHelpers.h"
#include "InventoryComponent.h"

int FInventoryHelpers::GetTotalItemCountForAllPlayers(const UWorld* World, const FString& ItemName)
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
				TotalItemCount += Inventory->GetItemCount(ItemName);
			}
		}
	}

	return TotalItemCount;
}
