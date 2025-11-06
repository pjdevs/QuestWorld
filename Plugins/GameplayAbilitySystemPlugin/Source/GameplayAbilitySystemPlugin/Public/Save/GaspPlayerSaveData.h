// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GaspPlayerSaveData.generated.h"

class UGameplayAbility;

/**
 * 
 */
USTRUCT()
struct GAMEPLAYABILITYSYSTEMPLUGIN_API FGaspPlayerSaveData
{
	GENERATED_BODY();

public:
	UPROPERTY()
	TSet<TSubclassOf<UGameplayAbility>> GrantedAbilities;
};
