// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QIPAttributeWidgetController.h"
#include "Blueprint/UserWidget.h"
#include "QIPAttributePanel.generated.h"

class UQIPAttributeWidgetController;
/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UQIPAttributePanel : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;

public:
	void SetWidgetController(UQIPAttributeWidgetController* InWidgetController);
	
protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Ability)
	void SetHealth(float CurrentHealth, float MaxHealth);
	
	UFUNCTION(BlueprintImplementableEvent, Category = Ability)
	void SetStamina(float CurrentStamina, float MaxStamina);

private:
	UPROPERTY()
	TObjectPtr<UQIPAttributeWidgetController> WidgetController;

	FDelegateHandle HealthDelegateHandle;
	FDelegateHandle StaminaDelegateHandle;
};
