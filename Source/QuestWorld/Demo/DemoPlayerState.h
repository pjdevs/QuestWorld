// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GaspPlayerState.h"
#include "Save/SavablePlayer.h"
#include "DemoPlayerState.generated.h"

class UInventoryComponent;
/**
 * 
 */
UCLASS()
class QUESTWORLD_API ADemoPlayerState : public AGaspPlayerState, public ISavablePlayer
{
	GENERATED_BODY()

public:
	ADemoPlayerState();

public: // ISavablePlayer interface
	virtual int GetPlayerIndex() override;
	
protected:
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void SeamlessTravelTo(APlayerState* NewPlayerState) override;

public:
	int PlayerIndex = 0;
	bool bIsInit = false;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Inventory)
	UInventoryComponent* PlayerInventory;
};
