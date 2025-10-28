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
	FIPInteractiveState State = FIPInteractiveState();
};