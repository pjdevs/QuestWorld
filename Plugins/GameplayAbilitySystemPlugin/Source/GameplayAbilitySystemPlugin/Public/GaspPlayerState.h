// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GaspPlayerState.generated.h"

class UGaspAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITYSYSTEMPLUGIN_API AGaspPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGaspPlayerState();
	
public: // IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	TObjectPtr<UGaspAbilitySystemComponent> AbilitySystemComponent;
};
