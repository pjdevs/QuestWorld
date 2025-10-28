// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractiveState.generated.h"

UENUM(BlueprintType)
enum class EIPInteractiveState : uint8
{
	Ready, // can be interacted
	Busy, // is currently in interaction so cannot do anything else
	Interacted, // definitively interacted,
	Destroyed // interactive will be or has been destroyed
};

/**
 * Represent the current state of an interactive (state + interaction count + more later).
 */
USTRUCT(BlueprintType)
struct FIPInteractiveState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	EIPInteractiveState State = EIPInteractiveState::Ready;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	uint8 InteractionCount = 0;
};