// Copyright pjdevs. All Rights Reserved.


#include "UI/AttributesWidget.h"

#include "GaspAttributeSet.h"
#include "GaspPlayerState.h"

// TODO Make generic widget to give one attribute and its max and display on change etc

void UAttributesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const AGaspPlayerState* PlayerState = GetOwningPlayerState<AGaspPlayerState>();

	if (!IsValid(PlayerState))
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	const UGaspAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

	if (!IsValid(AbilitySystemComponent) ||!IsValid(AttributeSet))
	{
		return;
	}

	SetHealth(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
	SetStamina(AttributeSet->GetStamina(), AttributeSet->GetMaxStamina());

	HealthAttributeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetHealthAttribute()
	)
		.AddUObject(this, &UAttributesWidget::OnAttributeChanged);

	MaxHealthAttributeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxHealthAttribute()
	)
		.AddUObject(this, &UAttributesWidget::OnAttributeChanged);
	
	StaminaAttributeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetStaminaAttribute()
	)
		.AddUObject(this, &UAttributesWidget::OnAttributeChanged);

	MaxStaminaAttributeChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetMaxStaminaAttribute()
	)
		.AddUObject(this, &UAttributesWidget::OnAttributeChanged);
}

void UAttributesWidget::NativeDestruct()
{
	Super::NativeDestruct();

	const AGaspPlayerState* PlayerState = GetOwningPlayerState<AGaspPlayerState>();

	if (!IsValid(PlayerState))
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	const UGaspAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

	if (!IsValid(AbilitySystemComponent) ||!IsValid(AttributeSet))
	{
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetHealthAttribute()
	)
		.Remove(HealthAttributeChangedDelegateHandle);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
		.Remove(MaxHealthAttributeChangedDelegateHandle);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute())
		.Remove(StaminaAttributeChangedDelegateHandle);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute())
		.Remove(MaxStaminaAttributeChangedDelegateHandle);
}

void UAttributesWidget::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	const AGaspPlayerState* PlayerState = GetOwningPlayerState<AGaspPlayerState>();

	if (!IsValid(PlayerState))
	{
		return;
	}

	const UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	const UGaspAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

	if (!IsValid(AbilitySystemComponent) ||!IsValid(AttributeSet))
	{
		return;
	}

	if (
		Data.Attribute == AttributeSet->GetHealthAttribute()
		|| Data.Attribute == AttributeSet->GetMaxHealthAttribute()
	)
	{
		SetHealth(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
	}
	else if (
		Data.Attribute == AttributeSet->GetStaminaAttribute()
		|| Data.Attribute == AttributeSet->GetMaxStaminaAttribute()
	)
	{
		SetStamina(AttributeSet->GetStamina(), AttributeSet->GetMaxStamina());
	}
}
