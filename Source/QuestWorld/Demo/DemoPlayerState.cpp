// Copyright pjdevs. All Rights Reserved.


#include "DemoPlayerState.h"

#include "InventoryComponent.h"


ADemoPlayerState::ADemoPlayerState()
{
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>("PlayerInventory");
}

FString ADemoPlayerState::OverrideName_Implementation() const
{
	return FString::Printf(TEXT("PlayerState%d"), PlayerIndex);
}
