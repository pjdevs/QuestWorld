// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemId.h"
#include "Assets/QuestObjective.h"
#include "InventoryObjective.generated.h"

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UInventoryObjective : public UQuestObjective
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
	FInventoryItemId TargetItemId = FInventoryItemId();
	
	UPROPERTY(EditDefaultsOnly, Category = Inventory, meta = (AllowPrivateAccess = true))
	int TargetItemCount = 1;
};
