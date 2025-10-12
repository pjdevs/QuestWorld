// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractive.h"
#include "IPInteractorComponent.h"
#include "IPInteractiveActor.generated.h"

class UBoxComponent;

UENUM()
enum class EIPInteractiveState : uint8
{
	Ready,
	Interacted
};

/**
 * A base interactive Actor implementing IInteractive interface.
 */
UCLASS(Abstract)
class INTERACTIONPLUGIN_API AIPInteractiveActor : public AActor, public IIPInteractive
{
	GENERATED_BODY()
	
public:
	AIPInteractiveActor();

	virtual void Interact(AActor* InteractionInstigator) override;
	virtual bool CanBeInteracted(AActor* InteractionInstigator) const override;
	virtual FVector GetInteractiveLocation() const override;
	virtual UWidgetComponent* GetWidgetComponent() const override;
	virtual TSubclassOf<UIPInteractionWidget> GetInteractionWidgetClass() const override;
	virtual TSubclassOf<UUserWidget> GetIndicationWidgetClass() const override;
	virtual TSubclassOf<UUserWidget> GetIndicationBlockedWidgetClass() const override;
	virtual FText GetInteractiveName() const override;
	virtual FText GetInteractionDescription() const override;
	virtual bool IsAutoInteractive() const override;

	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	/**
	 * Action to execute on the server for interaction.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void DoInteraction(AActor* InteractionInstigator);
	virtual void DoInteraction_Implementation(AActor* InteractionInstigator);
	
	/**
	 * Feedback to execute on the client for interaction.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void DoFeedback();
	virtual void DoFeedback_Implementation();

	/**
	 * Blueprintable conditions in addition to core ones to decide whether this actor can be interacted or not.
	 */
	UFUNCTION(BlueprintNativeEvent)
	bool CanBeInteractedBy(AActor* InteractionInstigator) const;
	virtual bool CanBeInteractedBy_Implementation(AActor* InteractionInstigator) const;

	/**
	 * Function that can be called to notify interactors that state may have changed
	 * (to update can be interacted condition etc.).
	 * Called on client and/or server (after replication). 
	 */
	UFUNCTION(BlueprintCallable)
	void StateChanged();

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

private:
	UFUNCTION()
	void OnRep_State();
	
private:
	/**
	 * Trigger component used for interaction.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> InteractionTrigger;

	/**
	 * Trigger component showing indicator of possible interaction. Should be larger than InteractionTrigger!
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> IndicationTrigger;

	/**
	 * The widget component to display interaction widgets inside.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> WidgetComponent;
	
	/**
	 * Class of the interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TSubclassOf<UIPInteractionWidget> InteractionWidgetClass;

	/**
	 * Class of the indication widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> IndicationWidgetClass;

	/**
	 * Class of the indication widget when interaction is not possible.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> IndicationBlockedWidgetClass;
	
	/**
	 * Whether this actor can be interacted one time or multiple times.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	bool bInteractMultipleTimes;

	/**
	 * Name of the interactive actor to show in the interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	FText InteractiveName;
	
	/**
	 * Description of the interaction to show in the interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	FText InteractionDescription;

	/**
	 * Whether to auto interact with the first interactor entering trigger area.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	bool bAutoInteract;

private:
	/**
	 * Interaction state of the actor.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction, ReplicatedUsing=OnRep_State, meta = (AllowPrivateAccess = true))
	EIPInteractiveState State;

	/**
	 * The array of possible interactors that have been indicated.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TArray<TWeakObjectPtr<UIPInteractorComponent>> IndicatedInteractors;
};
