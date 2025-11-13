// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractionStatus.h"
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
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = Interaction)
	void OnStartInteractionInput(AActor* InteractionInstigator);
	
	/**
	 * Action to execute on the server for beginning of interaction input.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = Interaction)
	void OnEndInteractionInput(AActor* InteractionInstigator);

	/**
	 * Decide whether this actor can be interacted or not.
	 * Will be used on both clients and sever for validation.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
	FIPInteractionStatus GetInteractionStatusForActor(AActor* InteractionInstigator);
};
