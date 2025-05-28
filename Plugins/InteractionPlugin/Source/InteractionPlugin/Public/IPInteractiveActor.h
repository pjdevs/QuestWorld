// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
	/**
	 * @brief Trigger component used for interaction.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	UBoxComponent* InteractionTrigger;

	/**
	 ** Whether this actor can be interacted one time or multiple times.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	bool InteractMultipleTimes;

	/**
	 ** Description of the interaction to show in the interaction widget.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	FString InteractionDescription;

	/**
	 ** Whether to auto interact with the first interactor entering trigger area.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	bool AutoInteract;
	
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

	UFUNCTION()
	void OnRep_State();

	/**
	 * Action to execute on the server for interaction.
	 */
	virtual void DoInteract(AActor* InteractionInstigator);
	/**
	 * Action to execute on the server for interaction (Blueprint version).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void BP_DoInteract(AActor* InteractionInstigator);
	virtual void BP_DoInteract_Implementation(AActor* InteractionInstigator);
	
	/**
	 * Feeback to execute on the client for interaction.
	 */
	virtual void DoFeedback();
	/**
	 * Feeback to execute on the client for interaction (Blueprint version).
	 */
	UFUNCTION(BlueprintNativeEvent)
	void BP_DoFeedback();
	virtual void BP_DoFeedback_Implementation();
	
	UFUNCTION()
	void HandleTriggerBeginOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I, bool bArg, const FHitResult& HitResult);
	UFUNCTION()
	void HandleTriggerEndOverlap(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1, int I);

	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	AIPInteractiveActor();

	virtual void Interact(AActor* InteractionInstigator) override;
	virtual bool CanBeInteracted(AActor* InteractionInstigator) override;
	virtual FVector GetInteractiveLocation() const override { return GetActorLocation(); };
	virtual FString GetInteractionDescription() const override;
	virtual bool IsAutoInteractive() const override;
};
