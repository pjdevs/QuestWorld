// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogEvents.h"
#include "IPInteractiveActor.h"
#include "DialogActor.generated.h"

class UDialogGraphAsset;

UCLASS()
class QUESTINTEGRATIONPLUGIN_API ADialogActor : public AIPInteractiveActor, public IDialogEvents
{
	GENERATED_BODY()

public:
	ADialogActor();
	
public: // Interactive actor
	virtual void OnStartInteractionInput_Implementation(AActor* InteractionInstigator) override;
	virtual FIPInteractionStatus GetInteractionStatus(AActor* InteractionInstigator) const override;

public: // IDalogEvents interface
	virtual void OnDialogStarted(AController* DialogController) override;
	virtual void OnDialogEnded(AController* DialogController) override;

private:
	// TODO Later handle soft ref etc in dialog like quest
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	TObjectPtr<UDialogGraphAsset> DialogAsset;

	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	FText InDialogReasonText;
};
