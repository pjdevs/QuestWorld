// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributesWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITYSYSTEMPLUGIN_API UAttributesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetHealth(float HealthValue, float MaxHealthValue);

	UFUNCTION(BlueprintImplementableEvent)
	void SetStamina(float StaminaValue, float MaxStaminaValue);
};
