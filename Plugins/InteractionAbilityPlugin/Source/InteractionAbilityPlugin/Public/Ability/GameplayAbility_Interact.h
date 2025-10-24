// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Interact.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIONABILITYPLUGIN_API UGameplayAbility_Interact : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Interact();

	const FText& GetAbilityName() const { return AbilityName; }

protected:
		virtual void ActivateAbility(
			const FGameplayAbilitySpecHandle Handle,
			const FGameplayAbilityActorInfo* ActorInfo,
			const FGameplayAbilityActivationInfo ActivationInfo,
			const FGameplayEventData* TriggerEventData
		) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Ability, meta = (AllowPrivateAccess = true))
	FText AbilityName;
};
