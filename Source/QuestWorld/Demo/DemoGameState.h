// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISpudObject.h"
#include "GameFramework/GameStateBase.h"
#include "DemoGameState.generated.h"

class UInventoryComponent;
class UQuestComponent;

/**
 * 
 */
UCLASS()
class QUESTWORLD_API ADemoGameState : public AGameStateBase, public ISpudObject
{
	GENERATED_BODY()

public:
	ADemoGameState();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Quest)
	TObjectPtr<UQuestComponent> QuestComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = Quest)
	TObjectPtr<UInventoryComponent> SharedInventory;
};
