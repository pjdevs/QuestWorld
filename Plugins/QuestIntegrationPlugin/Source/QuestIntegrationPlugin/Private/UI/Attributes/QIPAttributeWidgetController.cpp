// Copyright pjdevs. All Rights Reserved.


#include "UI/Attributes/QIPAttributeWidgetController.h"
#include "AbilitySystemComponent.h"
#include "GaspAttributeSet.h"
#include "GaspPlayerState.h"

void UQIPAttributeWidgetController::InitializeWithPlayerState(APlayerState* InPlayerState)
{
	AGaspPlayerState* PlayerState = Cast<AGaspPlayerState>(InPlayerState);

	if (!IsValid(PlayerState))
	{
		return;
	}
	
	AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	AttributeSet = PlayerState->GetAttributeSet();

	if (!IsValid(AbilitySystemComponent) ||!IsValid(AttributeSet))
	{
		return;
	}

	HealthAttributeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetHealthAttribute()
	)
		.AddUObject(this, &UQIPAttributeWidgetController::OnAttributeChanged);

	MaxHealthAttributeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxHealthAttribute()
	)
		.AddUObject(this, &UQIPAttributeWidgetController::OnAttributeChanged);
	
	StaminaAttributeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetStaminaAttribute()
	)
		.AddUObject(this, &UQIPAttributeWidgetController::OnAttributeChanged);

	MaxStaminaAttributeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxStaminaAttribute()
	)
		.AddUObject(this, &UQIPAttributeWidgetController::OnAttributeChanged);
	
}

void UQIPAttributeWidgetController::Cleanup()
{
	if (!IsValid(AbilitySystemComponent) || !IsValid(AttributeSet))
	{
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetHealthAttribute()
	)
		.Remove(HealthAttributeChangedDelegateHandle);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxHealthAttribute()
	)
		.Remove(MaxHealthAttributeChangedDelegateHandle);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetStaminaAttribute()
	)
		.Remove(StaminaAttributeChangedDelegateHandle);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxStaminaAttribute()
	)
		.Remove(MaxStaminaAttributeChangedDelegateHandle);
}

void UQIPAttributeWidgetController::TriggerInitialEvents()
{
	if (IsValid(AttributeSet))
	{
		OnHealthChanged.Broadcast(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
		OnStaminaChanged.Broadcast(AttributeSet->GetStamina(), AttributeSet->GetMaxStamina());
	}
}

void UQIPAttributeWidgetController::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (!IsValid(AttributeSet))
	{
		return;
	}

	if (
		Data.Attribute == AttributeSet->GetHealthAttribute()
		|| Data.Attribute == AttributeSet->GetMaxHealthAttribute()
	)
	{
		OnHealthChanged.Broadcast(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
	}
	else if (
		Data.Attribute == AttributeSet->GetStaminaAttribute()
		|| Data.Attribute == AttributeSet->GetMaxStaminaAttribute()
	)
	{
		OnStaminaChanged.Broadcast(AttributeSet->GetStamina(), AttributeSet->GetMaxStamina());
	}
}
