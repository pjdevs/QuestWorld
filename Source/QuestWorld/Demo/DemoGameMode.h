// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DemoGameMode.generated.h"

/**
 * 
 */
UCLASS()
class QUESTWORLD_API ADemoGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	int NextPlayerIndex = 0;
};
