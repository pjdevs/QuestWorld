// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPInteractiveState.h"
#include "InteractiveSaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FInteractiveSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	EIPInteractiveState State = EIPInteractiveState::Ready;

	UPROPERTY()
	bool bWasDestroyed = false;
};