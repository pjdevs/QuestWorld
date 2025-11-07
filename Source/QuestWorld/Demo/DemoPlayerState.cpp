// Copyright pjdevs. All Rights Reserved.


#include "DemoPlayerState.h"

#include "InventoryComponent.h"


ADemoPlayerState::ADemoPlayerState()
{
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>("PlayerInventory");
}

int ADemoPlayerState::GetPlayerIndex()
{
	return PlayerIndex;
}

void ADemoPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	if (ADemoPlayerState* NewDemoPlayerState = Cast<ADemoPlayerState>(PlayerState))
	{
		NewDemoPlayerState->bIsInit = bIsInit;
		NewDemoPlayerState->PlayerIndex = PlayerIndex;
	}
}

void ADemoPlayerState::SeamlessTravelTo(APlayerState* NewPlayerState)
{
	Super::SeamlessTravelTo(NewPlayerState);

	if (const ADemoPlayerState* NewDemoPlayerState = Cast<ADemoPlayerState>(NewPlayerState))
	{
		const FInventorySaveData InventorySave = PlayerInventory->WriteItemsToSave();
		NewDemoPlayerState->PlayerInventory->LoadItemsFromSave(InventorySave);
	}
}
