// Copyright pjdevs. All Rights Reserved.


#include "QuestWorldSaveSubsystem.h"

#include "IPWorldStateSaveSubsystem.h"
#include "InventoryComponent.h"
#include "InventoryStatics.h"
#include "QuestComponent.h"
#include "QuestStatics.h"
#include "QuestWorldSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UQuestWorldSaveSubsystem::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0))
	{
		SaveGameObject = Cast<UQuestWorldSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));
	}

	if (!SaveGameObject)
	{
		SaveGameObject = Cast<UQuestWorldSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UQuestWorldSaveGame::StaticClass())
		);
	}
}

void UQuestWorldSaveSubsystem::LoadGame()
{
	if (!SaveGameObject)
	{
		LoadSaveGame();
	}
	
	if (UInventoryComponent* SharedInventory = UInventoryStatics::GetSharedInventory(GetWorld()))
	{
		SharedInventory->LoadItemsFromSave(SaveGameObject->InventorySaveData);
	}

	if (UQuestComponent* QuestComponent = UQuestStatics::GetQuestComponent(GetWorld()))
	{
		QuestComponent->LoadQuestsFromSave(SaveGameObject->QuestSaveData);
	}

	if (UIPWorldStateSaveSubsystem* InteractiveSave = GetWorld()->GetSubsystem<UIPWorldStateSaveSubsystem>())
	{
		InteractiveSave->LoadSaveData(SaveGameObject->InteractiveWorldSaveData);
	}
}

void UQuestWorldSaveSubsystem::SaveGame()
{
	if (!SaveGameObject)
	{
		SaveGameObject = Cast<UQuestWorldSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UQuestWorldSaveGame::StaticClass())
		);
	}
	
	if (UInventoryComponent* SharedInventory = UInventoryStatics::GetSharedInventory(GetWorld()))
	{
		SaveGameObject->InventorySaveData = SharedInventory->WriteItemsToSave();
	}

	if (UQuestComponent* QuestComponent = UQuestStatics::GetQuestComponent(GetWorld()))
	{
		SaveGameObject->QuestSaveData = QuestComponent->WriteQuestsToSave();
	}

	if (UIPWorldStateSaveSubsystem* InteractiveSave = GetWorld()->GetSubsystem<UIPWorldStateSaveSubsystem>())
	{
		SaveGameObject->InteractiveWorldSaveData = InteractiveSave->GetSaveData();
	}

	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);
}
