// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractionStatus.h"
#include "IPInteractiveState.h"
#include "UObject/Interface.h"
#include "IPInteractionHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UIPInteractionHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONPLUGIN_API IIPInteractionHandler
{
	GENERATED_BODY()

public:
	/**
	 * Action to execute on the server for beginning of interaction input.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
	void OnStartInteractionInput(AActor* InteractionInstigator);
	
	/**
	 * Action to execute on the server for beginning of interaction input.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
	void OnEndInteractionInput(AActor* InteractionInstigator);
	
	/**
	 * Feedback to execute on the client when state changed.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
	void DoInteractionFeedback(const FIPInteractiveState& NewState);

	/**
	 * Additional conditions to core ones to decide whether this actor can be interacted or not.
	 * These conditions should be specific to one child type.
	 * If you subclass this to another type of interactive actor that can also be subclassed,
	 * use native GetInteractionStatus.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
	FIPInteractionStatus GetInteractionStatusForActor(
		AActor* InteractionInstigator,
		const FIPInteractiveState& CurrentState
	);
};
