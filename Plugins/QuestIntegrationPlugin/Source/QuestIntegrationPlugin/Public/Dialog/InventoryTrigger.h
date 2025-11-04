// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogTrigger.h"
#include "InventoryItemId.h"
#include "InventoryTrigger.generated.h"

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UInventoryTrigger : public UDialogTrigger
{
	GENERATED_BODY()

public:
	virtual void OnExecute_Implementation(AController* DialogController) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	FInventoryItemId ItemId = FInventoryItemId();

	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	int ItemCount = 1;

	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	bool bShouldRemove = false;

	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	bool bShouldUseSharedInventory = true;
};
