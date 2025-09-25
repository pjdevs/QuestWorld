// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IPInteractorComponent.generated.h"

class UInputAction;
class UIPInteractionWidget;
class IIPInteractive;

/**
 * ActorComponent for handling replicated interactions with other interactive actors. 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INTERACTIONPLUGIN_API UIPInteractorComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UIPInteractorComponent();
	
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

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

protected:
	/**
	 * Execute Interact on the server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_Interact();

	/**
	 * Show interaction widget on client.
	 * @param Interactive The interactive actor with which component can interact.
	 */
	UFUNCTION(Client, Reliable)
	void ShowInteractionWidget_Client(AActor* Interactive);

	/**
	 * Hide interaction widget on client.
	 * @param Interactive The interactive actor with which component could interact be can't anymore.
	 */
	UFUNCTION(Client, Reliable)
	void HideInteractionWidget_Client(AActor* Interactive);

private:
	/**
	 * Execute the line sweep and update most relevant actor by distance.
	 */
	void RecomputeInteractiveRelevancy();

	/**
	 * Called internally when has authority when most relevant actor changed (to show widgets etc).
	 */
	void OnMostRelevantActorChanged(AActor* PreviousMostRelevantActor, AActor* NewMostRelevantActor);

private:
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
	 * Input Action used to interact, to show in interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	UInputAction* InteractionAction;
	
private:
	/**
	 * The list of all current possible interactives.
	 */
	TArray<IIPInteractive*> PossibleInteractives;
};
