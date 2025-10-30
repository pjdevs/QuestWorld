// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


/**
 * State for stateful actors.
 */
UENUM(BlueprintType)
enum class EIPState : uint8
{
	Idle,
	Activating,
	Activated,
	Deactivating
};
