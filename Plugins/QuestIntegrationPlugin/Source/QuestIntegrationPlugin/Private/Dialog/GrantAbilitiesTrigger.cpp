// Copyright pjdevs. All Rights Reserved.


#include "Dialog/GrantAbilitiesTrigger.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpec.h"

void UGrantAbilitiesTrigger::OnExecute_Implementation(AController* DialogController)
{
	 if (const IAbilitySystemInterface* Asi = Cast<IAbilitySystemInterface>(DialogController->GetPawn()))
	 {
		if (UAbilitySystemComponent* AbilitySystemComponent = Asi->GetAbilitySystemComponent())
		{
	 		for (const TSubclassOf<UGameplayAbility>& AbilityClass : AbilitiesToGrant)
			{
				const FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}
	 }
}
