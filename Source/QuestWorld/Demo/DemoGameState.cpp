// Copyright pjdevs. All Rights Reserved.


#include "DemoGameState.h"

#include "InventoryComponent.h"
#include "QuestComponent.h"


ADemoGameState::ADemoGameState()
{
	QuestComponent = CreateDefaultSubobject<UQuestComponent>("Quest Component");
	SharedInventory = CreateDefaultSubobject<UInventoryComponent>("Shared Inventory Component");
}
