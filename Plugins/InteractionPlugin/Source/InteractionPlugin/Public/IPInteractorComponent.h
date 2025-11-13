// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/WeakInterfacePtr.h"
#include "Components/ActorComponent.h"
#include "IPInteractorComponent.generated.h"

class UIPInteractiveComponent;
class UInputAction;
class UIPInteractionWidget;

// TODO Only make calculations to show widget etc on client and find way to validate distance on server.
// TODO Make a normal to the interactive and if angle with the normal is too high disable interaction, etc.

/**
 * Helper struct for representing information on the computed interaction score of an interactive.
 */
struct FIPInteractionScore
{
	float InteractionScore;
	float AngleFromTarget;
	float DistanceFromTarget;
};

/**
 * ActorComponent for handling replicated interactions with other interactive actors. 
 */
UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class INTERACTIONPLUGIN_API UIPInteractorComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UIPInteractorComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

public:
	/** Called for start interaction input. */
	UFUNCTION(BlueprintCallable)
	void TryStartInteractionInput();

	/** Called for end interaction input */
	UFUNCTION(BlueprintCallable)
	void TryEndInteractionInput();

	/**
	 * Get the input action required to interact.
	 */
	UFUNCTION(BlueprintPure)
	UInputAction* GetInteractionAction() const { return InteractionAction; }
	
	/**
	 * Add an interactive to the list of possible interactives.
	 */
	void AddInteractive(UIPInteractiveComponent* Interactive);

	/**
	 * Remove an interactive from the list of possible interactives.
	 */
	void RemoveInteractive(UIPInteractiveComponent* Interactive);

	/**
	 * Add an indicator to the interactive to notify that is it interactive but not close enough.
	 */
	void AddInteractiveIndication(UIPInteractiveComponent* Interactive);

	/**
	 * Remove indicator from the interactive.
	 */
	void RemoveInteractiveIndication(UIPInteractiveComponent* Interactive);

	/**
	 * Called by an interactive when its state has changed and this interactor was in range.
	 */
	void OnInteractiveStatusChanged(UIPInteractiveComponent* Interactive);

	/**
	 * Get the current most relevant actor that interactor would interact with in TryInteract.
	 */
	TWeakObjectPtr<UIPInteractiveComponent> GetMostRelevantInteractive() const;

protected:
	/**
	 * Execute Interact on the server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_TryStartInteractionInput();

	/**
	 * Execute Interact on the server.
	 */
	UFUNCTION(Server, Reliable)
	void Server_TryEndInteractionInput();

	/**
	 * Show interaction widget on client.
	 * @param Interactive The interactive actor with which component can interact.
	 */
	void ShowInteractionWidgetClient(UIPInteractiveComponent* Interactive);

	/**
	 * Show indication widget on client.
	 * @param Interactive The interactive actor on which we should show indication.
	 */
	void ShowIndicationWidgetClient(UIPInteractiveComponent* Interactive);
	
	/**
	 * Hide interaction widget on client.
	 * @param Interactive The interactive actor on which we should hide the widget.
	 */
	void HideWidgetClient(UIPInteractiveComponent* Interactive);

	/**
	 * Helper to check if we are on a local pawn.
	 */
	bool IsLocal() const;

private:
	/**
	 * Recompute the relevancy of each interactive by checking look angle, distance etc.
	 */
	void RecomputeInteractiveRelevancy(bool bForceRefresh = false);

	/**
	 * Remove all possibly destroyed object that was in the possibly interactive array.
	 */
	void PurgeInvalidInteractives();
	
	/**
	 * Recompute the relevancy of each interactive by checking look angle, distance etc.
	 */
	UIPInteractiveComponent* FindNewMostRelevantActor() const;

	// TODO expose this to designers?
	/**
	 * Compute interaction score for a given interactive. Used to sort them and find the most relevant one.
	 * The higher the score, the better.
	 */
	static FIPInteractionScore ComputeInteractionScore(
		const UIPInteractiveComponent& Target,
		const FVector& EyesLocation,
		const FVector& LookDirection
	);
	
	/**
	 * Called internally when has authority when most relevant actor changed (to show widgets etc).
	 */
	void OnMostRelevantInteractiveChanged(
		UIPInteractiveComponent* PreviousMostRelevantInteractive,
		UIPInteractiveComponent* NewMostRelevantInteractive
	);

private:
	/**
	 * Maximum interaction distance. Will discard interactives further than this distance, even if in trigger zone.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	float MaxInteractionDistance;

	/**
	 * Maximum interaction angle. Will discard interactives with greater angle from look vector, even if in trigger zone.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	float MaxInteractionAngleDegrees;

	/**
	 * Input Action used to interact, to show in interaction widget.
	 */
	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractionAction;
	
private:
	/**
	 * The list of all current possible interactives.
	 */
	TSet<TWeakObjectPtr<UIPInteractiveComponent>> PossibleInteractives;

	/**
	 * The list of all indicated interactives (in range but not close enough).
	 */
	TSet<TWeakObjectPtr<UIPInteractiveComponent>> IndicatedInteractives;

	/**
	 * The current most relevant interactive to interact with.
	 */
	TWeakObjectPtr<UIPInteractiveComponent> MostRelevantInteractive;
};
