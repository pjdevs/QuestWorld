// Copyright pjdevs. All Rights Reserved.


#include "UI/AttributesWidget.h"

#include "GaspAttributeSet.h"
#include "GaspPlayerState.h"

// TODO Make generic widget to give one attribute and its max and display on change etc

void UAttributesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const AGaspPlayerState* PlayerState = GetOwningPlayerState<AGaspPlayerState>();

	if (!PlayerState)
	{
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
	const UGaspAttributeSet* AttributeSet = PlayerState->GetAttributeSet();

	SetHealth(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
	SetStamina(AttributeSet->GetStamina(), AttributeSet->GetMaxStamina());

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
		.AddLambda([this, AttributeSet](const FOnAttributeChangeData& Data)
		{
			SetHealth(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
		.AddLambda([this, AttributeSet](const FOnAttributeChangeData& Data)
		{
			SetHealth(AttributeSet->GetHealth(), AttributeSet->GetMaxHealth());
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute())
		.AddLambda([this, AttributeSet](const FOnAttributeChangeData& Data)
		{
			SetStamina(AttributeSet->GetStamina(), AttributeSet->GetMaxStamina());
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute())
		.AddLambda([this, AttributeSet](const FOnAttributeChangeData& Data)
		{
			SetStamina(AttributeSet->GetStamina(), AttributeSet->GetMaxStamina());
		});
}
