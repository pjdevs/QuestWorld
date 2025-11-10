// Copyright pjdevs. All Rights Reserved.


#include "UI/Attributes/QIPAttributePanel.h"
#include "UI/Attributes/QIPAttributeWidgetController.h"

void UQIPAttributePanel::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(WidgetController))
	{
		WidgetController->OnHealthChanged.Remove(HealthDelegateHandle);
		WidgetController->OnStaminaChanged.Remove(StaminaDelegateHandle);
	}
}

void UQIPAttributePanel::SetWidgetController(UQIPAttributeWidgetController* InWidgetController)
{
	if (!IsValid(InWidgetController))
	{
		return;
	}
	
	WidgetController = InWidgetController;

	HealthDelegateHandle = WidgetController->OnHealthChanged.AddUObject(this, &UQIPAttributePanel::SetHealth);
	StaminaDelegateHandle = WidgetController->OnStaminaChanged.AddUObject(this, &UQIPAttributePanel::SetStamina);
	
	WidgetController->TriggerInitialEvents();
}
