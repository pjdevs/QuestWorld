// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemId.h"
#include "IPInteractiveActor.h"
#include "ItemInteractive.generated.h"

UCLASS()
class QUESTWORLD_API AItemInteractive : public AIPInteractiveActor
{
	GENERATED_BODY()

public:
	virtual FIPInteractionStatus GetExtraInteractionStatusForActor_Implementation(
		AActor* InteractionInstigator,
		EIPState CurrentState
	) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Interaction|Item", meta = (AllowPrivateAccess = true))
	FInventoryItemId RequiredItem;

	UPROPERTY(EditAnywhere, Category = "Interaction|Item", meta = (AllowPrivateAccess = true))
	TArray<FInventoryItemId> BonusHiddenItems;

	UPROPERTY(EditAnywhere, Category = "Interaction|Item", meta = (AllowPrivateAccess = true))
	FText RequiredItemTextFormat;
};
