// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GaspPlayerState.h"
#include "ISpudObject.h"
#include "DemoPlayerState.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class QUESTWORLD_API ADemoPlayerState : public AGaspPlayerState, public ISpudObject
{
	GENERATED_BODY()

public:
	ADemoPlayerState();

public:
	virtual FString OverrideName_Implementation() const override;

public:
	int PlayerIndex = 0;

	bool bIsInit = false;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame, Category = Inventory)
	UInventoryComponent* PlayerInventory;
};
