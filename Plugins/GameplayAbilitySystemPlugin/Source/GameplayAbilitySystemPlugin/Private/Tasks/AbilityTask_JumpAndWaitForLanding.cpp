// Copyright pjdevs. All Rights Reserved.


#include "Tasks/AbilityTask_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UAbilityTask_JumpAndWaitForLanding* UAbilityTask_JumpAndWaitForLanding::JumpAndWaitForLanding(
	UGameplayAbility* OwningAbility,
	ACharacter* OwnerCharacter
)
{
	auto* Task = NewAbilityTask<UAbilityTask_JumpAndWaitForLanding>(OwningAbility);
	Task->OwnerCharacter = OwnerCharacter;

	return Task;
}

void UAbilityTask_JumpAndWaitForLanding::Activate()
{
	if (!OwnerCharacter || !OwnerCharacter->CanJump())
	{
		EndTask();
		OnLanded.Broadcast(); // to output something even if there was no character (could stick ability)
		return;
	}

	OwnerCharacter->Jump();
	OwnerCharacter->MovementModeChangedDelegate.AddDynamic(this, &UAbilityTask_JumpAndWaitForLanding::OnMovementModeChanged);
}

void UAbilityTask_JumpAndWaitForLanding::OnMovementModeChanged(
	ACharacter* Character,
	EMovementMode PrevMovementMode,
	uint8 PreviousCustomMode
)
{
	if (Character->GetCharacterMovement()->MovementMode != MOVE_Falling)
	{
		CleanTask();
		EndTask();
		OnLanded.Broadcast();
	}
}

void UAbilityTask_JumpAndWaitForLanding::CleanTask()
{
	if (OwnerCharacter)
	{
		OwnerCharacter->MovementModeChangedDelegate.RemoveDynamic(this, &UAbilityTask_JumpAndWaitForLanding::OnMovementModeChanged);
	}
}
