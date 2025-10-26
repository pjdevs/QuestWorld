// Copyright pjdevs. All Rights Reserved.


#include "Components/CharacterMovementByTagComponent.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UCharacterMovementByTagComponent::UCharacterMovementByTagComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterMovementByTagComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	if (!OwnerCharacter)
	{
		return;
	}

	OwnerAsc = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerCharacter);

	if (!OwnerAsc)
	{
		return;
	}
	
	TagCountChangedDelegateHandle = OwnerAsc->RegisterGameplayTagEvent(MovementBlockedTag)
		.AddUObject(this, &UCharacterMovementByTagComponent::OnMovementBlockedTagCountChanged);
}

void UCharacterMovementByTagComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetOwnerRole() != ROLE_Authority ||!OwnerAsc || !OwnerAsc)
	{
		return;
	}

	OwnerAsc->UnregisterGameplayTagEvent(TagCountChangedDelegateHandle, MovementBlockedTag);
}

void UCharacterMovementByTagComponent::OnMovementBlockedTagCountChanged(FGameplayTag GameplayTag, int TagCount)
{
	if (!OwnerCharacter)
	{
		return;
	}
	
	if (TagCount > 0)
	{
		LastMovementMode = OwnerCharacter->GetCharacterMovement()->MovementMode;
		bLastUseControllerRotationYaw = OwnerCharacter->bUseControllerRotationYaw;
		
		OwnerCharacter->GetCharacterMovement()->DisableMovement();
		OwnerCharacter->bUseControllerRotationYaw = false;
	}
	else
	{
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(LastMovementMode);
		OwnerCharacter->bUseControllerRotationYaw = bLastUseControllerRotationYaw;
	}
}
