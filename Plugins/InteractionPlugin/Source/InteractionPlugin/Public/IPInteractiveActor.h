// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractionStatus.h"
#include "IPInteractionHandler.h"
#include "IPInteractorComponent.h"
#include "IPStatefulActor.h"
#include "IPInteractiveActor.generated.h"

class UWidgetComponent;
class UBoxComponent;

/**
 * A base interactive using stateful and interaction components.
 */
UCLASS(Abstract)
class INTERACTIONPLUGIN_API AIPInteractiveActor : public AIPStatefulActor, public IIPInteractionHandler
{
	GENERATED_BODY()
	
public:
	AIPInteractiveActor();

	virtual void PostInitializeComponents() override;

protected: // IIPStateHandler interface
	virtual FIPInteractionStatus GetInteractionStatusForActor_Implementation(AActor* InteractionInstigator) override;
	
protected:
	/**
	 * Start interaction phase which will make object currently
	 * interacting/unavailable until EndInteractionPhase is called.
	 * Will set StatefulComponent.State to Activating.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Interaction)
	void StartInteractionPhase(AActor* InteractionInstigator);

	/**
	 * End interaction phase making object available again.
	 * Can be called without calling StartInteractionPhase before.
	 * Enum effects :
	 * - Idle: will stay interactable.
	 * - Activating: do not use, will stuck object.
	 * - Activated: will not be interactive again.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Interaction)
	void EndInteractionPhase(EIPState NextState);

	/**
	 * Additional conditions to core ones to decide whether this actor can be interacted or not.
	 * These conditions should be specific to one child type.
	 * If you subclass this to another type of interactive actor that can also be subclassed,
	 * use native GetInteractionStatus.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
	FIPInteractionStatus GetExtraInteractionStatusForActor(
		AActor* InteractionInstigator,
		EIPState CurrentState
	) const;
	virtual FIPInteractionStatus GetExtraInteractionStatusForActor_Implementation(
		AActor* InteractionInstigator,
		EIPState CurrentState
	) const;

	/**
	 * Relay NotifyStatusChanged call to InteractiveComponent.
	 */
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void NotifyStatusChanged();

protected:
	/**
	 * Interactive component.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction)
	TObjectPtr<UIPInteractiveComponent> InteractiveComponent;

	/**
	 * Trigger component used for interaction.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction)
	TObjectPtr<UBoxComponent> InteractionTrigger;

	/**
	 * Trigger component showing indicator of possible interaction. Should be larger than InteractionTrigger or zero!
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction)
	TObjectPtr<UBoxComponent> IndicationTrigger;

	/**
	 * Interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction)
	TObjectPtr<UWidgetComponent> InteractionWidget;
};
