// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractive.h"
#include "IPInteractorComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/Object.h"
#include "IPInteractiveActor.generated.h"

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
	virtual bool CanBeInteracted(AActor* InteractionInstigator) override;
	FORCEINLINE virtual FVector GetInteractiveLocation() const override { return GetActorLocation(); }
	virtual FString GetInteractionDescription() const override;
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
	
	UFUNCTION()
	void HandleTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult
	);
	UFUNCTION()
	void HandleTriggerEndOverlap(
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
	 * @brief Trigger component used for interaction.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	UBoxComponent* InteractionTrigger;

	/**
	 * Whether this actor can be interacted one time or multiple times.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	bool bInteractMultipleTimes;

	/**
	 * Description of the interaction to show in the interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	FString InteractionDescription;

	/**
	 * Whether to auto interact with the first interactor entering trigger area.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	bool bAutoInteract;
	
	/**
	 * Interaction state of the actor.
	 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Interaction, ReplicatedUsing=OnRep_State, meta = (AllowPrivateAccess = true))
	EIPInteractiveState State;

	/**
	 * The array of possible interactors.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TArray<UIPInteractorComponent*> PossibleInteractors;
};
