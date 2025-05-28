// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IPInteractorComponent.generated.h"

class UIPInteractionWidget;
class IIPInteractive;

/**
 * Delegate for executing a line sweep.
 * @param Distance The distance of the line sweep.
 * @param TraceChannel The collision channel to use.
 * @param Hits The output hit results of the line sweep.
 */
DECLARE_DELEGATE_RetVal_ThreeParams(bool, FInteractionTraceDelegate, float /*Distance*/, ECollisionChannel /*TraceChannel*/, TArray<FHitResult>& /*Hits*/)

/**
 * ActorComponent for handling replicated interactions with other interactive actors. 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONPLUGIN_API UIPInteractorComponent : public UActorComponent
{
	GENERATED_BODY()
	
	/**
	 * Distance to trace for interaction.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction, meta = (AllowPrivateAccess = true))
	float InteractionDistance;

	/**
	 * Collision channel to use when interaction trace. 
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel;
	
	/**
	 * The current most relevant actor to interact with.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	AActor* MostRelevantActor;

	/**
	 ** Widget to that interactor will use to add to his viewport and describe the interaction.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TSubclassOf<UIPInteractionWidget> InteractionWidgetClass;
	
	/**
	 * Store the current widget for current possible interaction.
	 */
	UPROPERTY(VisibleInstanceOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	UIPInteractionWidget* InteractionWidget;

	/**
	 * The list of all current possible interactives.
	 */
	TArray<IIPInteractive*> PossibleInteractives;

	/**
	 * Delegate which will execute the line sweep.
	 */
	FInteractionTraceDelegate InteractionTraceDelegate;
	
public:
	// Sets default values for this component's properties
	UIPInteractorComponent();
	
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	/** Called for interact input */
	UFUNCTION(BlueprintCallable)
	void Interact();

	/**
	 * Add an interactive to the list of possible interactives.
	 */
	void AddInteractive(IIPInteractive* Interactive);

	/**
	 * Remove an interactive from the list of possible interactives.
	 */
	void RemoveInteractive(IIPInteractive* Interactive);

	/**
	 * Set the delegate for executing the interaction line sweep.
	 */
	void SetInteractionTraceDelegate(FInteractionTraceDelegate Delegate);

protected:
	/**
	 * Execute Interact on the server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_Interact();

	/**
	 * Execute the line sweep and update most relevant actor by distance.
	 */
	void RecomputeInteractiveRelevancy();
	
	UFUNCTION(Client, Reliable)
	void ShowInteractionWidget_Client(AActor* Interactive);
	
	UFUNCTION(Client, Reliable)
	void HideInteractionWidget_Client();
};
