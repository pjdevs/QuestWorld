// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuestWorldGameMode.h"
#include "QuestWorldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuestWorldGameMode::AQuestWorldGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
