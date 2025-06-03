// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractiveActor.h"
#include "DialogActor.generated.h"

class UDialogDataAsset;

UCLASS()
class QUESTINTEGRATIONPLUGIN_API ADialogActor : public AIPInteractiveActor
{
	GENERATED_BODY()

	// TODO Later handle soft ref etc in dialog like quest
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	TObjectPtr<UDialogDataAsset> DialogAsset;

public:
	ADialogActor();
	
	virtual void DoInteract(AActor* InteractionInstigator) override;
};
