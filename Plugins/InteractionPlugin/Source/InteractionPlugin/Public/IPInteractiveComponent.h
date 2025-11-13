// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractiveActor.h"
#include "Components/ActorComponent.h"
#include "IPInteractiveComponent.generated.h"

class UIPInteractorComponent;

UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class INTERACTIONPLUGIN_API UIPInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UIPInteractiveComponent();
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public: // UIPInteractiveComponent public interface

	/**
	 * Set interaction trigger to use.
	 * @param InInteractionTrigger 
	 */
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void SetInteractionTrigger(UPrimitiveComponent* InInteractionTrigger);

	/**
	 * Set indication trigger to use.
	 * @param InIndicationTrigger 
	 */
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void SetIndicationTrigger(UPrimitiveComponent* InIndicationTrigger);

	/**
	 * Set widget to use for displaying interaction prompt.
	 * @param InInteractionWidget 
	 */
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void SetInteractionWidget(UWidgetComponent* InInteractionWidget);
	
	/**
	 * Begin interaction input with the interactive object.
	 */
	void StartInteractionInput(AActor* InteractionInstigator);

	/**
	 * End interaction input with the interactive object.
	 */
	void EndInteractionInput(AActor* InteractionInstigator);

	/**
	 * Check whether the object can be interacted or not by giving information on interaction status. 
	 */
	FIPInteractionStatus GetInteractionStatusForActor(AActor* InteractionInstigator) const;

	/**
	 * Get interactive's location. 
	 */
	FVector GetInteractiveLocation() const;

	/**
	 * Get the in world widget component to display widgets inside. 
	 */
	UWidgetComponent* GetWidgetComponent() const;

	/**
	 * The class of interaction widget to use. 
	 */
	TSubclassOf<UIPInteractionWidget> GetInteractionWidgetClass() const;

	/**
	 * The class if indication widget to use. 
	 */
	TSubclassOf<UUserWidget> GetIndicationWidgetClass() const;

	/**
	 * The class if indication widget to use when interaction is not possible. 
	 */
	TSubclassOf<UUserWidget> GetIndicationBlockedWidgetClass() const;

	/**
	 * Get the description of the interaction the show in the interaction widget.
	 */
	FText GetInteractiveName() const;
	
	/**
	 * Get the description of the interaction the show in the interaction widget.
	 */
	FText GetInteractionDescription() const;

	/**
	 * Whether this interactive is auto interactive.
	 */
	bool IsAutoInteractive() const;

	/**
	 * Start interaction phase which will make object currently
	 * interacting/unavailable until EndInteractionPhase is called.
	 * Will set InteractiveState.State to Busy.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Interaction)
	void StartInteractionPhase(AActor* InteractionInstigator);

	/**
	 * End interaction phase making object available again.
	 * Can be called without calling StartInteractionPhase before.
	 * Enum effects :
	 * - Ready: will stay interactable.
	 * - Busy: do not use, will stuck object (unless this is called again with something else).
	 * - Interacted: will not be interactive again.
	 * - Destroy: will replicate the state to clients and then destroy.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Interaction)
	void EndInteractionPhase();

	/**
	 * Function that can be called to notify interactors that state may have changed
	 * (to update can be interacted condition etc.).
	 * Should be called on both client and server in OnRep_ (to handle new conditions and widgets).
	 */
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void NotifyStatusChanged();

protected:
	UFUNCTION()
	void HandleInteractionTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult
	);

	UFUNCTION()
	void HandleInteractionTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	void HandleIndicationTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult
	);

	UFUNCTION()
	void HandleIndicationTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex
	);

	void PurgeInvalidInteractors();
	
protected:
	/**
	 * Class of the interaction widget.
	 */
	UPROPERTY(EditAnywhere, Category = Interaction)
	TSubclassOf<UIPInteractionWidget> InteractionWidgetClass;

	/**
	 * Class of the indication widget.
	 */
	UPROPERTY(EditAnywhere, Category = Interaction)
	TSubclassOf<UUserWidget> IndicationWidgetClass;

	/**
	 * Class of the indication widget when interaction is not possible.
	 */
	UPROPERTY(EditAnywhere, Category = Interaction)
	TSubclassOf<UUserWidget> IndicationBlockedWidgetClass;

	/**
	 * Name of the interactive actor to show in the interaction widget.
	 */
	UPROPERTY(EditAnywhere, Category = Interaction)
	FText InteractiveName;
	
	/**
	 * Description of the interaction to show in the interaction widget.
	 */
	UPROPERTY(EditAnywhere, Category = Interaction)
	FText InteractionDescription;

	/**
	 * Whether to auto interact with the first interactor entering trigger area.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	bool bIsAutoInteractive;

protected:
	/**
	 * Trigger component used for interaction.
	 */
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> InteractionTrigger;

	/**
	 * Trigger component showing indicator of possible interaction. Should be larger than InteractionTrigger!
	 */
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> IndicationTrigger;
	
	/**
	 * The widget component to display interaction widgets inside.
	 */
	UPROPERTY()
	TObjectPtr<UWidgetComponent> InteractionWidget;

	/**
	 * The array of interactors that have been indicated.
	 */
	TSet<TWeakObjectPtr<UIPInteractorComponent>> IndicatedInteractors;

	/**
	 * The array of possible interactors.
	 */
	TSet<TWeakObjectPtr<UIPInteractorComponent>> PossibleInteractors;

	/**
	 * Current interactor that began interaction input.
	 */
	TWeakObjectPtr<AActor> CurrentInteractorActor; 
};
