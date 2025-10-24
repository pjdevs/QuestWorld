// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractionStatus.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType, Category = Interaction)
struct INTERACTIONPLUGIN_API FIPInteractionStatus
{
	GENERATED_BODY()

	/**
	 * Can the actor be interacted?
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	bool bCanBeInteracted = false;

	/**
	 * Reason text to be displayed if it cannot be interacted.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	FText ReasonText = FText::GetEmpty();
};
