// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveSaveData.h"
#include "InteractiveWorldSaveData.generated.h"

USTRUCT()
struct FInteractiveWorldSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FName, FInteractiveSaveData> SavedInteractives = TMap<FName, FInteractiveSaveData>();
};