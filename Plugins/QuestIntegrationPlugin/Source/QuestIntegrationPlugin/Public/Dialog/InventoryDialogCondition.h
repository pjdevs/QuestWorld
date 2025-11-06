// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogCondition.h"
#include "InventoryItemId.h"
#include "InventoryDialogCondition.generated.h"

// TODO Replace that with a QuestStepCondition
// (to see if completed Balgruf quest step 1 rather than manually checking same conditions)

/**
 * 
 */
UENUM(BlueprintType)
enum class EInventoryConditionBehavior : uint8
{
	More UMETA(DisplayName = "More Items"),
	Less UMETA(DisplayName = "Less Items"),
};

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UInventoryDialogCondition : public UDialogCondition
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied_Implementation(AController* DialogController) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	EInventoryConditionBehavior Behavior = EInventoryConditionBehavior::More;
	
	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	FInventoryItemId TargetItemId = FInventoryItemId();
	
	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	int TargetItemCount = 1;

	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	bool bShouldUseSharedInventory = true;
};
