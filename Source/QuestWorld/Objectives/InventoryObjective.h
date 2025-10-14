// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Assets/QuestObjective.h"
#include "InventoryObjective.generated.h"

/**
 * 
 */
UCLASS()
class QUESTWORLD_API UInventoryObjective : public UQuestObjective
{
	GENERATED_BODY()

public:
	UInventoryObjective();

protected:
	virtual int GetCompletion_Implementation(UWorld* World) override;
	virtual int GetTargetValue_Implementation() override;
	virtual int TriggerProgress_Implementation(UWorld* World, UBaseQuestEvent* Event) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	FPrimaryAssetId TargetItemId = FPrimaryAssetId();
	
	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	int TargetItemCount = 1;
};
