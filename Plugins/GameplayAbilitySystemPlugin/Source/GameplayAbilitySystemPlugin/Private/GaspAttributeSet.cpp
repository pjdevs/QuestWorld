// Copyright pjdevs. All Rights Reserved.


#include "GaspAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UGaspAttributeSet::UGaspAttributeSet()
{
}

void UGaspAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGaspAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGaspAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGaspAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGaspAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}

void UGaspAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());	
	}

	if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());	
	}
}

void UGaspAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float HealthValue = GetHealth();
		const float MaxHealthValue = GetMaxHealth();

		if (HealthValue <= 0.0f)
		{
			SetHealth(0.0f);
		}
		else if (HealthValue > MaxHealthValue)
		{
			SetHealth(MaxHealthValue);	
		}
	}

	if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		const float StaminaValue = GetStamina();
		const float MaxStaminaValue = GetMaxStamina();

		if (StaminaValue <= 0.0f)
		{
			SetStamina(0.0f);
		}
		else if (StaminaValue > MaxStaminaValue)
		{
			SetStamina(MaxStaminaValue);	
		}
	}
}

void UGaspAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGaspAttributeSet, Health, OldHealth);
}

void UGaspAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGaspAttributeSet, MaxHealth, OldMaxHealth);
}

void UGaspAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGaspAttributeSet, Stamina, OldStamina);
}

void UGaspAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGaspAttributeSet, MaxStamina, OldMaxStamina);
}


