// Copyright pjdevs. All Rights Reserved.


#include "DemoGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "DemoPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Save/IntegrationSaveSubsystem.h"

AActor* ADemoGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	ADemoPlayerState* PlayerState = Player->GetPlayerState<ADemoPlayerState>();

	if (!PlayerState)
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}

	if (!PlayerState->bIsInit)
	{
		NextPlayerIndex++;
		PlayerState->PlayerIndex = NextPlayerIndex;
		PlayerState->bIsInit = true;
	}
	
	TArray<AActor*> SpawnPoints {};
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);
	
	FString StartPrefix = UGameplayStatics::ParseOption(OptionsString, "Start");
	
	if (StartPrefix.IsEmpty())
	{
		StartPrefix = "Start";
	}

	const FName TagToSearch = FName(*FString::Printf(
		TEXT("%s%d"),
		*StartPrefix,
		PlayerState->PlayerIndex
	));
	
	for (AActor* SpawnPoint : SpawnPoints)
	{
		if (const APlayerStart* PlayerStart = Cast<APlayerStart>(SpawnPoint))
		{
			if (PlayerStart->PlayerStartTag == TagToSearch)
			{
				return SpawnPoint;
			}
		}
	}

	return nullptr;
}

void ADemoGameMode::PostLogin(APlayerController* NewPlayer)
{
	if (ADemoPlayerState* DemoPlayerState = NewPlayer->GetPlayerState<ADemoPlayerState>())
	{
		if (UIntegrationSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UIntegrationSaveSubsystem>())
		{
			SaveSubsystem->LoadPlayer(DemoPlayerState, DemoPlayerState->PlayerIndex);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Player state not found in OnPostLogin."));
	}

	// do this after because it will possess pawn so load state before pawn possession
	Super::PostLogin(NewPlayer);
}

void ADemoGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	// if (const ADemoPlayerState* DemoPlayerState = Exiting->GetPlayerState<ADemoPlayerState>())
	// {
	// 	if (UIntegrationSaveSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<UIntegrationSaveSubsystem>())
	// 	{
	// 		SaveSubsystem->SavePlayer(DemoPlayerState, DemoPlayerState->PlayerIndex);
	// 	}
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("Player state not found in Logout."));
	// }
}

