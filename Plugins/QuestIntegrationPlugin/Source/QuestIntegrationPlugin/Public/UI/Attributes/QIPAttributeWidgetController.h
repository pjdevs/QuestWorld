// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "UObject/Object.h"
#include "QIPAttributeWidgetController.generated.h"

class AGaspPlayerState;
class UGaspAttributeSet;
class APlayerState;

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FAttributeChangedDelegate,
	float, /* CurrentAttributeValue */
	float /* MaxAttributeValue */
);

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UQIPAttributeWidgetController : public UObject
{
	GENERATED_BODY()

public:
	void InitializeWithPlayerState(APlayerState* InPlayerState);
	void Cleanup();
	void TriggerInitialEvents();

public:
	FAttributeChangedDelegate OnHealthChanged;
	FAttributeChangedDelegate OnStaminaChanged;

private:
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
	
private:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UGaspAttributeSet> AttributeSet;	

	FDelegateHandle HealthAttributeChangedDelegateHandle;
	FDelegateHandle MaxHealthAttributeChangedDelegateHandle;
	FDelegateHandle StaminaAttributeChangedDelegateHandle;
	FDelegateHandle MaxStaminaAttributeChangedDelegateHandle;
};
