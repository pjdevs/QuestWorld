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
