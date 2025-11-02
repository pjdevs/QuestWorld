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


static TAutoConsoleVariable<FString> CVarWorldStateSaveGameSlot(
	TEXT("Save.SaveGameSlot"),
	"QuestWorldSave",
	TEXT("Defines the slot to use for save subsystem.\n")
);

static FAutoConsoleCommand Cmd_Save_Reset(
	TEXT("Save.Reset"),
	TEXT("Reset save of the world state subsystem."),
	FConsoleCommandWithArgsDelegate::CreateLambda(
		[](const TArray<FString>& Args)
		{
			const FString& SlotName = CVarWorldStateSaveGameSlot->GetString();
			
			if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
			{
				UGameplayStatics::DeleteGameInSlot(SlotName, 0);
			}
		}
	)
);

static FAutoConsoleCommand Cmd_Save_Save(
	TEXT("Save.Save"),
	TEXT("Execute a new save of the game."),
	FConsoleCommandWithWorldArgsAndOutputDeviceDelegate::CreateLambda(
		[](const TArray<FString>& Args, UWorld* World, FOutputDevice& Ar)
		{
			if (UIntegrationSaveSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<UIntegrationSaveSubsystem>())
			{
				SaveSubsystem->SaveGame();
			}
		}
	)
);

static FAutoConsoleCommand Cmd_Save_Load(
	TEXT("Save.Load"),
	TEXT("Load a save of the game."),
	FConsoleCommandWithWorldArgsAndOutputDeviceDelegate::CreateLambda(
		[](const TArray<FString>& Args, UWorld* World, FOutputDevice& Ar)
		{
			if (Args.IsEmpty())
			{
				UE_LOG(LogTemp, Error, TEXT("Need to provide a valid slot name arg."));
				return;
			}
			
			if (UIntegrationSaveSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<UIntegrationSaveSubsystem>())
			{
				SaveSubsystem->LoadSaveGame(Args[0]);
				SaveSubsystem->LoadGame();
			}
		}
	)
);

void UIntegrationSaveSubsystem::LoadSaveGameFromConfig()
{
	const FString& SlotName = CVarWorldStateSaveGameSlot->GetString();
	LoadSaveGame(SlotName);
}

void UIntegrationSaveSubsystem::LoadGame()
{
	if (!SaveGameObject)
	{
		LoadSaveGameFromConfig();
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
		const FIPWorldStateSaveData InteractiveWorldSaveData = InteractiveSave->GetSaveData();
		SaveGameObject->InteractiveWorldSaveData.SavedActors.Append(InteractiveWorldSaveData.SavedActors);
	}

	if (UPersistentActionsComponent* PersistentActions = UPersistentActionsStatics::GetPersistentActions(GetWorld()))
	{
		SaveGameObject->ActionsDone = PersistentActions->GetActionsDone();
	}

	const FString& SlotName = CVarWorldStateSaveGameSlot->GetString();
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, 0);
}

void UIntegrationSaveSubsystem::LoadSaveGame(const FString& SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		SaveGameObject = Cast<UIntegrationSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	}

	if (!SaveGameObject)
	{
		SaveGameObject = Cast<UIntegrationSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UIntegrationSaveGame::StaticClass())
		);
	}
}
