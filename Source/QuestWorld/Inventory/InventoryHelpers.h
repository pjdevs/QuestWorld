// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class QUESTWORLD_API FInventoryHelpers
{
public:
	static int GetTotalItemCountForAllPlayers(const UWorld* World, const FString& ItemName);
};
