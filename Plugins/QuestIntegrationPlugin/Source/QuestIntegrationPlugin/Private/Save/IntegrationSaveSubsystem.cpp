// Copyright pjdevs. All Rights Reserved.


#include "Save/IntegrationSaveSubsystem.h"
#include "Save/IntegrationSaveGame.h"
#include "IPWorldStateSaveSubsystem.h"
#include "InventoryComponent.h"
#include "InventoryStatics.h"
#include "QuestComponent.h"
#include "QuestStatics.h"
#include "Actions/PersistentActionsComponent.h"
#include "Actions/PersistentActionsStatics.h"
#include "Kismet/GameplayStatics.h"

void UIntegrationSaveSubsystem::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlotName, 0))
	{
		SaveGameObject = Cast<UIntegrationSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameSlotName, 0));
	}

	if (!SaveGameObject)
	{
		SaveGameObject = Cast<UIntegrationSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UIntegrationSaveGame::StaticClass())
		);
	}
}

void UIntegrationSaveSubsystem::LoadGame()
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

	if (UPersistentActionsComponent* PersistentActions = UPersistentActionsStatics::GetPersistentActions(GetWorld()))
	{
		PersistentActions->SetActionsDone(SaveGameObject->ActionsDone);
	}
}

void UIntegrationSaveSubsystem::SaveGame()
{
	if (!SaveGameObject)
	{
		SaveGameObject = Cast<UIntegrationSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UIntegrationSaveGame::StaticClass())
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
		SaveGameObject->InteractiveWorldSaveData.SavedActors.Append(InteractiveSave->GetSaveData().SavedActors);
	}

	if (UPersistentActionsComponent* PersistentActions = UPersistentActionsStatics::GetPersistentActions(GetWorld()))
	{
		SaveGameObject->ActionsDone = PersistentActions->GetActionsDone();
	}

	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameSlotName, 0);
}

void UIntegrationSaveSubsystem::SetSaveGameSlotName(const FString& SlotName)
{
	SaveGameSlotName = SlotName;
}
