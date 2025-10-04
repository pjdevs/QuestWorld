// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractiveActor.h"
#include "DialogActor.generated.h"

class UDialogGraphAsset;

UCLASS()
class QUESTINTEGRATIONPLUGIN_API ADialogActor : public AIPInteractiveActor
{
	GENERATED_BODY()

public:
	ADialogActor();
	
	virtual void DoInteraction_Implementation(AActor* InteractionInstigator) override;

private:
	// TODO Later handle soft ref etc in dialog like quest
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	TObjectPtr<UDialogGraphAsset> DialogAsset;
};
