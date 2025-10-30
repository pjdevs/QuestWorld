// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPStateSaveData.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FIPStateSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 State = 0;

	UPROPERTY()
	uint8 Counter = 0;

	UPROPERTY()
	bool bWasDestroyed = false;
};