// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemId.h"
#include "IPInteractiveActor.h"
#include "CollectableItem.generated.h"

UCLASS()
class QUESTWORLD_API ACollectableItem : public AIPInteractiveActor
{
	GENERATED_BODY()

public:
	ACollectableItem();
	
	virtual void OnStartInteractionInput_Implementation(AActor* InteractionInstigator) override;

private:
	UPROPERTY(EditAnywhere, Category = Interaction, meta = (AllowPrivateAccess = true))
	FInventoryItemId GivesItem;
};
