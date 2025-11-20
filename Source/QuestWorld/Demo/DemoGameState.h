// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DemoGameState.generated.h"

class UQuestComponent;
/**
 * 
 */
UCLASS()
class QUESTWORLD_API ADemoGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ADemoGameState();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Quest)
	UQuestComponent* QuestComponent;
};
