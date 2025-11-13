// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GaspPlayerState.h"
#include "DemoPlayerState.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class QUESTWORLD_API ADemoPlayerState : public AGaspPlayerState
{
	GENERATED_BODY()

public:
	ADemoPlayerState();

public:
	virtual FString OverrideName_Implementation() const override;

public:
	UPROPERTY(BlueprintReadOnly, Category = PlayerState)
	int PlayerIndex = 0;

	bool bIsInit = false;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame, Category = Inventory)
	TObjectPtr<UInventoryComponent> PlayerInventory;
};
