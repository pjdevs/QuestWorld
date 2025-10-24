// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractionStatus.h"
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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	virtual void Interact(AActor* InteractionInstigator) override;
	virtual FIPInteractionStatus GetInteractionStatus(AActor* InteractionInstigator) const override;
	virtual FVector GetInteractiveLocation() const override;
	virtual UWidgetComponent* GetWidgetComponent() const override final;
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
	 * Additional conditions to core ones to decide whether this actor can be interacted or not.
	 * These conditions should be specific to one child type.
	 * If you subclass this to another type of interactive actor that can also be subclassed,
	 * use native GetInteractionStatus.
	 */
	UFUNCTION(BlueprintNativeEvent)
	FIPInteractionStatus GetInteractionStatusForActor(AActor* InteractionInstigator) const;
	virtual FIPInteractionStatus GetInteractionStatusForActor_Implementation(AActor* InteractionInstigator) const;

	/**
	 * Function that can be called to notify interactors that state may have changed
	 * (to update can be interacted condition etc.).
	 * Should be called on both client and server in OnRep_ (to handle new conditions and widgets).
	 */
	UFUNCTION(BlueprintCallable, Category = Interaction)
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
	
protected:
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	bool bAutoInteract;

private:
	/**
	 * Interaction state of the actor.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction, ReplicatedUsing=OnRep_State, meta = (AllowPrivateAccess = true))
	EIPInteractiveState State;

	/**
	 * The array of interactors that have been indicated.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TArray<TWeakObjectPtr<UIPInteractorComponent>> IndicatedInteractors;

	/**
	 * The array of possible interactors.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TArray<TWeakObjectPtr<UIPInteractorComponent>> PossibleInteractors;
};
