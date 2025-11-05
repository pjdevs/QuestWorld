// Copyright pjdevs. All Rights Reserved.


#include "Dialog/GrantAbilitiesTrigger.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GrantAbility.h"

class IGrantAbility;

void UGrantAbilitiesTrigger::OnExecute_Implementation(AController* DialogController)
{
	 if (const IAbilitySystemInterface* Asi = Cast<IAbilitySystemInterface>(DialogController->GetPawn()))
	 {
		if (const UAbilitySystemComponent* AbilitySystemComponent = Asi->GetAbilitySystemComponent())
		{
			if (IGrantAbility* UnlockAbility = Cast<IGrantAbility>(AbilitySystemComponent->GetOwnerActor()))
			
	 		for (const TSubclassOf<UGameplayAbility>& AbilityClass : AbilitiesToGrant)
			{
	 			UnlockAbility->GrantAbility(AbilityClass);
			}
		}
	 }
}
