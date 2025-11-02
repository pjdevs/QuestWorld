// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "AttributesWidget.generated.h"

struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class GAMEPLAYABILITYSYSTEMPLUGIN_API UAttributesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetHealth(float HealthValue, float MaxHealthValue);

	UFUNCTION(BlueprintImplementableEvent)
	void SetStamina(float StaminaValue, float MaxStaminaValue);

private:
	void OnAttributeChanged(const FOnAttributeChangeData& Data);

private:
	FDelegateHandle HealthAttributeChangedDelegateHandle;
	FDelegateHandle MaxHealthAttributeChangedDelegateHandle;
	FDelegateHandle StaminaAttributeChangedDelegateHandle;
	FDelegateHandle MaxStaminaAttributeChangedDelegateHandle;
};
