// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GrantAbility.generated.h"

class UGameplayAbility;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UGrantAbility : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEPLAYABILITYSYSTEMPLUGIN_API IGrantAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual void GrantAbility(TSubclassOf<UGameplayAbility> AbilityClass) = 0;
};
