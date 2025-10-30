// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPStateSaveData.h"
#include "IPWorldStateSaveData.generated.h"

USTRUCT()
struct FIPWorldStateSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<FName, FIPStateSaveData> SavedActors = TMap<FName, FIPStateSaveData>();
};