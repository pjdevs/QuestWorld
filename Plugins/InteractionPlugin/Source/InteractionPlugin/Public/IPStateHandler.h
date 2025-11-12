// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPState.h"
#include "UObject/Interface.h"
#include "IPStateHandler.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UIPStateHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONPLUGIN_API IIPStateHandler
{
	GENERATED_BODY()

public:
	/**
	 * Callback executed on server when state changed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintAuthorityOnly, Category = Interaction)
	void OnStateChangedServer(EIPState OldState, EIPState NewState);

	/**
	 * Callback executed on client when state change.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = Interaction)
	void OnStateChangedClient(EIPState OldState, EIPState NewState);
};
