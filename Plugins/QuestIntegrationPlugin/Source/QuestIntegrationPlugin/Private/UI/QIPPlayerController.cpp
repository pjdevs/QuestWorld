// Copyright pjdevs. All Rights Reserved.


#include "UI/QIPPlayerController.h"
#include "UI/QIPHUDLayout.h"
#include "Blueprint/UserWidget.h"

void AQIPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		HUDLayout = CreateWidget<UQIPHUDLayout>(this, HUDWidgetClass);

		if (HUDLayout)
		{
			HUDLayout->AddToViewport();

			if (UWorld* World = GetWorld())
			{
				if (AGameStateBase* GameState = World->GetGameState())
				{
					OnGameStateSet(GameState);
				}
				else
				{
					GameStateSetHandle = World->GameStateSetEvent.AddUObject(this, &AQIPPlayerController::OnGameStateSet);
				}
			}

			if (PlayerState)
			{
				HUDLayout->SetPlayerState(PlayerState);
			}
		}
	}
}

void AQIPPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsLocalController())
	{
		if (HUDLayout)
		{
			HUDLayout->RemoveFromParent();
			HUDLayout = nullptr;
		}

		if (
			UWorld* World = GetWorld();
			World && GameStateSetHandle.IsValid()
		)
		{
			World->GameStateSetEvent.Remove(GameStateSetHandle);
		}
	}
}

void AQIPPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (HUDLayout)
	{
		HUDLayout->SetPlayerState(PlayerState);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HUDLayout was nullptr while setting PlayerState."));
	}
}

void AQIPPlayerController::OnGameStateSet(AGameStateBase* SetGameState)
{
	if (HUDLayout)
	{
		HUDLayout->SetGameState(SetGameState);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HUDLayout was nullptr while setting GameState."));
	}
}
