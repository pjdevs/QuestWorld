// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveSaveData.h"
#include "IPInteractionStatus.h"
#include "IPInteractive.h"
#include "IPInteractorComponent.h"
#include "IPSavableInteractive.h"
#include "IPInteractiveActor.generated.h"

class UBoxComponent;

/**
 * A base interactive Actor implementing IInteractive interface.
 */
UCLASS(Abstract)
class INTERACTIONPLUGIN_API AIPInteractiveActor : public AActor, public IIPInteractive, public IIPSavableInteractive
{
	GENERATED_BODY()
	
public:
	AIPInteractiveActor();

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public: // IIPInteractive interface
	virtual void StartInteractionInput(AActor* InteractionInstigator) override;
	virtual void EndInteractionInput(AActor* InteractionInstigator) override;
	virtual FIPInteractionStatus GetInteractionStatus(AActor* InteractionInstigator) const override;
	virtual FVector GetInteractiveLocation() const override;
	virtual UWidgetComponent* GetWidgetComponent() const override final;
	virtual TSubclassOf<UIPInteractionWidget> GetInteractionWidgetClass() const override;
	virtual TSubclassOf<UUserWidget> GetIndicationWidgetClass() const override;
	virtual TSubclassOf<UUserWidget> GetIndicationBlockedWidgetClass() const override;
	virtual FText GetInteractiveName() const override;
	virtual FText GetInteractionDescription() const override;
	virtual bool IsAutoInteractive() const override;

public: // IIPSavableInteractive interface
	virtual FName GetUniqueId() const override;
	virtual bool IsSavable() const override;
	virtual void LoadFromSave(const FInteractiveSaveData& InteractiveSaveData) override;
	virtual FInteractiveSaveData WriteToSave() override;
	
protected:
	/**
	 * Action to execute on the server for beginning of interaction input.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly)
	void OnStartInteractionInput(AActor* InteractionInstigator);
	virtual void OnStartInteractionInput_Implementation(AActor* InteractionInstigator);

	/**
	 * Action to execute on the server for beginning of interaction input.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly)
	void OnEndInteractionInput(AActor* InteractionInstigator);
	virtual void OnEndInteractionInput_Implementation(AActor* InteractionInstigator);
	
	/**
	 * Feedback to execute on the client when state changed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic)
	void DoFeedback();
	virtual void DoFeedback_Implementation();

	/**
	 * Start interaction phase which will make object currently
	 * interacting/unavailable until EndInteractionPhase is called.
	 * Will set InteractiveState.State to Busy.
	 */
	UFUNCTION(BlueprintCallable)
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
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void EndInteractionPhase(EIPInteractiveState NextState);

	/**
	 * Additional conditions to core ones to decide whether this actor can be interacted or not.
	 * These conditions should be specific to one child type.
	 * If you subclass this to another type of interactive actor that can also be subclassed,
	 * use native GetInteractionStatus.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly)
	FIPInteractionStatus GetInteractionStatusForActor(AActor* InteractionInstigator) const;
	virtual FIPInteractionStatus GetInteractionStatusForActor_Implementation(AActor* InteractionInstigator) const;

	/**
	 * Function that can be called to notify interactors that state may have changed
	 * (to update can be interacted condition etc.).
	 * Should be called on both client and server in OnRep_ (to handle new conditions and widgets).
	 */
	UFUNCTION(BlueprintCallable, Category = Interaction)
	void NotifyStateChanged();

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
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TObjectPtr<UBoxComponent> InteractionTrigger;

	/**
	 * Trigger component showing indicator of possible interaction. Should be larger than InteractionTrigger!
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TObjectPtr<UBoxComponent> IndicationTrigger;

	/**
	 * The widget component to display interaction widgets inside.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TObjectPtr<UWidgetComponent> WidgetComponent;
	
	/**
	 * Class of the interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TSubclassOf<UIPInteractionWidget> InteractionWidgetClass;

	/**
	 * Class of the indication widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TSubclassOf<UUserWidget> IndicationWidgetClass;

	/**
	 * Class of the indication widget when interaction is not possible.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TSubclassOf<UUserWidget> IndicationBlockedWidgetClass;

	/**
	 * Name of the interactive actor to show in the interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	FText InteractiveName;
	
	/**
	 * Description of the interaction to show in the interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	FText InteractionDescription;

	/**
	 * Whether to auto interact with the first interactor entering trigger area.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction)
	bool bAutoInteract;

	/**
	 * Save unique ID of this interactive.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Save")
	FName UniqueId;
	
	/**
	 * Whether this interactive should be saved or not.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Save")
	bool bIsSavable;

protected:
	/**
	 * Interaction state of the actor.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Interaction, ReplicatedUsing=OnRep_State)
	FIPInteractiveState InteractiveState;

	/**
	 * Current interactor that began interaction input.
	 */
	TWeakObjectPtr<AActor> CurrentInteractor; 

	/**
	 * The array of interactors that have been indicated.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction)
	TArray<TWeakObjectPtr<UIPInteractorComponent>> IndicatedInteractors;

	/**
	 * The array of possible interactors.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction)
	TArray<TWeakObjectPtr<UIPInteractorComponent>> PossibleInteractors;
};
