// Copyright pjdevs. All Rights Reserved.


#include "Save/IntegrationSaveSubsystem.h"
#include "GaspPlayerState.h"
#include "Save/IntegrationSaveGame.h"
#include "IPWorldStateSaveSubsystem.h"
#include "InventoryComponent.h"
#include "InventoryStatics.h"
#include "QuestComponent.h"
#include "QuestStatics.h"
#include "Actions/PersistentActionsComponent.h"
#include "Actions/PersistentActionsStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Save/UIntegrationPlayerSaveGame.h"


static TAutoConsoleVariable<FString> CVarWorldStateSaveGameSlot(
	TEXT("Save.SaveGameSlot"),
	"WorldStateSave",
	TEXT("Defines the slot to use for save subsystem (world).\n")
);

static TAutoConsoleVariable<FString> CVarPlayerStateSaveGameSlot(
	TEXT("Save.PlayerSaveGameSlot"),
	"PlayerStateSave",
	TEXT("Defines the slot to use for save subsystem (players).\n")
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

void UIntegrationSaveSubsystem::LoadPlayerSaveGame(const FString& SlotName, int PlayerIndex)
{
	const FString& PlayerSlotName = GetPlayerSlotName(SlotName, PlayerIndex);
	UUIntegrationPlayerSaveGame* PlayerSaveGameObject = nullptr;
	
	if (UGameplayStatics::DoesSaveGameExist(PlayerSlotName, 0))
	{
		PlayerSaveGameObject = Cast<UUIntegrationPlayerSaveGame>(
			UGameplayStatics::LoadGameFromSlot(SlotName, 0)
		);
	}

	if (!PlayerSaveGameObject)
	{
		PlayerSaveGameObject = Cast<UUIntegrationPlayerSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UUIntegrationPlayerSaveGame::StaticClass())
		);
	}

	PlayerSaveGameObjects.Add(PlayerIndex, PlayerSaveGameObject);
}

void UIntegrationSaveSubsystem::LoadPlayerSaveGameFromConfig(int PlayerIndex)
{
	return LoadPlayerSaveGame(CVarPlayerStateSaveGameSlot->GetString(), PlayerIndex);
}

void UIntegrationSaveSubsystem::LoadPlayer(APlayerState* PlayerState, int PlayerIndex)
{
	UUIntegrationPlayerSaveGame* PlayerSaveGameObject = PlayerSaveGameObjects.FindRef(PlayerIndex);
	
	if (!PlayerSaveGameObject)
	{
		LoadPlayerSaveGameFromConfig(PlayerIndex);
	}

	PlayerSaveGameObject = PlayerSaveGameObjects.FindChecked(PlayerIndex);
	
	if (AGaspPlayerState* GaspPlayerState = Cast<AGaspPlayerState>(PlayerState))
	{
		GaspPlayerState->LoadFromSave(PlayerSaveGameObject->GaspSaveData);
	}

	if (UInventoryComponent* InventoryComponent = UInventoryStatics::GetPlayerInventory(PlayerState))
	{
		InventoryComponent->LoadItemsFromSave(PlayerSaveGameObject->InventorySaveData);
	}
}

void UIntegrationSaveSubsystem::SavePlayerSaveGame(const APlayerState* PlayerState, int PlayerIndex)
{
	UUIntegrationPlayerSaveGame* PlayerSaveGameObject = PlayerSaveGameObjects.FindRef(PlayerIndex);

	if (!PlayerSaveGameObject)
	{
		PlayerSaveGameObject = Cast<UUIntegrationPlayerSaveGame>(
			UGameplayStatics::CreateSaveGameObject(UUIntegrationPlayerSaveGame::StaticClass())
		);
	}

	if (const AGaspPlayerState* GaspPlayerState = Cast<AGaspPlayerState>(PlayerState))
	{
		PlayerSaveGameObject->GaspSaveData = GaspPlayerState->WriteToSave();
	}

	if (UInventoryComponent* InventoryComponent = UInventoryStatics::GetPlayerInventory(PlayerState))
	{
		PlayerSaveGameObject->InventorySaveData = InventoryComponent->WriteItemsToSave();
	}

	PlayerSaveGameObjects.Add(PlayerIndex, PlayerSaveGameObject);
}

void UIntegrationSaveSubsystem::SavePlayers()
{
	for (auto& [PlayerIndex, PlayerSaveGameObject] : PlayerSaveGameObjects)
	{
		const FString& SlotName = GetPlayerSlotName(CVarPlayerStateSaveGameSlot->GetString(), PlayerIndex);
		UGameplayStatics::SaveGameToSlot(PlayerSaveGameObject, SlotName, 0);
	}
}

FString UIntegrationSaveSubsystem::GetPlayerSlotName(const FString& SlotName, int PlayerIndex)
{
	return FString::Printf(
		TEXT("%s_%d"),
		*SlotName,
		PlayerIndex
	);
}
