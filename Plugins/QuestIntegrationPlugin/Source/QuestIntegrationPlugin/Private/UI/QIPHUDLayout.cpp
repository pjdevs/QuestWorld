// Copyright pjdevs. All Rights Reserved.


#include "UI/QIPHUDLayout.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "UI/Attributes/QIPAttributePanel.h"
#include "UI/Attributes/QIPAttributeWidgetController.h"
#include "UI/Mission/QIPMissionPanel.h"
#include "UI/Mission/QIPMissionWidgetController.h"


void UQIPHUDLayout::NativeDestruct()
{
	Super::NativeDestruct();

	if (AttributeController)
	{
		AttributeController->Cleanup();
		AttributeController = nullptr;
	}

	if (MissionController)
	{
		MissionController->Cleanup();
		MissionController = nullptr;
	}
}

void UQIPHUDLayout::SetGameState(AGameStateBase* InGameState)
{
	if (!IsValid(InGameState))
	{
		return;
	}
	
	GameState = InGameState;

	if (MissionPanel)
	{
		MissionController = NewObject<UQIPMissionWidgetController>(this);
		MissionController->InitializeWithGameState(GameState);
		MissionPanel->SetWidgetController(MissionController);
	}
}

void UQIPHUDLayout::SetPlayerState(APlayerState* InPlayerState)
{
	if (!IsValid(InPlayerState))
	{
		return;
	}

	PlayerState = InPlayerState;

	if (AttributePanel)
	{
		AttributeController = NewObject<UQIPAttributeWidgetController>(this);
		AttributeController->InitializeWithPlayerState(PlayerState);
		AttributePanel->SetWidgetController(AttributeController);
	}
}

