// Copyright pjdevs. All Rights Reserved.


#include "DemoGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "DemoPlayerState.h"
#include "Kismet/GameplayStatics.h"

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
	Super::PostLogin(NewPlayer);
}

void ADemoGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

