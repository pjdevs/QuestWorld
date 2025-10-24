// Copyright pjdevs. All Rights Reserved.


#include "Interactive/AbilityInteractiveActor.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "Ability/GameplayAbility_Interact.h"


AAbilityInteractiveActor::AAbilityInteractiveActor()
	: DoesNotHasRequiredAbilityTextFormat(FText::FromString("{0} needed")),
	CannotActivateRequiredAbilityTextFormat(FText::FromString("Cannot {0} now"))
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAbilityInteractiveActor::OnPreAbilityInteract(const FGameplayAbilitySpecHandle& AbilitySpecHandle)
{
	if (AbilitySpecHandle.IsValid())
	{
		CurrentAbilitySpecHandle = AbilitySpecHandle;
	}
}

void AAbilityInteractiveActor::OnPostAbilityInteract()
{
	CurrentAbilitySpecHandle = FGameplayAbilitySpecHandle();
}

void AAbilityInteractiveActor::Interact(AActor* InteractionInstigator)
{
	// First validate that we interact with the right ability 

	const UAbilitySystemComponent* InstigatorAsc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InteractionInstigator);

	if (!InstigatorAsc)
	{
		return;
	}
	
	const FGameplayAbilitySpec* Spec = InstigatorAsc->FindAbilitySpecFromHandle(CurrentAbilitySpecHandle);

	if (!Spec)
	{
		return; // handle forged or not belonging to this ASC
	}

	if (RequiredAbilityClass && !Spec->Ability->IsA(RequiredAbilityClass))
	{
		return;
	}

	Super::Interact(InteractionInstigator);
}

FIPInteractionStatus AAbilityInteractiveActor::GetInteractionStatus(AActor* InteractionInstigator) const
{
	const FIPInteractionStatus BaseInteractionStatus = Super::GetInteractionStatus(InteractionInstigator);

	if (!RequiredAbilityClass)
	{
		return BaseInteractionStatus;
	}

	if (!BaseInteractionStatus.bCanBeInteracted)
	{
		return BaseInteractionStatus;
	}

	const FText& RequiredAbilityName = RequiredAbilityClass.GetDefaultObject()->GetAbilityName(); 
	FIPInteractionStatus DoesNotHaveRequiredAbilityStatus = FIPInteractionStatus
	{
		.bCanBeInteracted = false,
		.ReasonText = FText::Format(DoesNotHasRequiredAbilityTextFormat, RequiredAbilityName)
	};
	FIPInteractionStatus CannotActivateRequiredAbilityStatus = FIPInteractionStatus
	{
		.bCanBeInteracted = false,
		.ReasonText = FText::Format(CannotActivateRequiredAbilityTextFormat, RequiredAbilityName)
	};

	const UAbilitySystemComponent* AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(InteractionInstigator);

	if (!AbilitySystemComponent)
	{
		return DoesNotHaveRequiredAbilityStatus;
	}

	const FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(RequiredAbilityClass);

	if (!Spec)
	{
		return DoesNotHaveRequiredAbilityStatus;
	}

	if (!Spec->Ability->CanActivateAbility(Spec->Handle, AbilitySystemComponent->AbilityActorInfo.Get()))
	{
		return CannotActivateRequiredAbilityStatus;
	}
	
	return FIPInteractionStatus { .bCanBeInteracted = true };
}

