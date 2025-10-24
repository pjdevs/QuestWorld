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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	
	virtual void DoInteraction_Implementation(AActor* InteractionInstigator) override;
	virtual FIPInteractionStatus GetInteractionStatus(AActor* InteractionInstigator) const override;

	// For now dialog system is not replicated so these events are only called on client
	
	virtual void OnDialogStarted(AController* DialogController) override;
	virtual void OnDialogEnded(AController* DialogController) override;

private:
	UFUNCTION()
	void OnRep_bIsInDialog();

private:
	// TODO Later handle soft ref etc in dialog like quest
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	TObjectPtr<UDialogGraphAsset> DialogAsset;

	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	FText InDialogReasonText;
	
private:
	UPROPERTY(ReplicatedUsing=OnRep_bIsInDialog)
	bool bIsInDialog;
};
