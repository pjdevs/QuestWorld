// Copyright pjdevs. All Rights Reserved.


#include "Tasks/AbilityTask_JumpAndWaitForLanding.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


UAbilityTask_JumpAndWaitForLanding* UAbilityTask_JumpAndWaitForLanding::JumpAndWaitForLanding(
	UGameplayAbility* OwningAbility
)
{
	return NewAbilityTask<UAbilityTask_JumpAndWaitForLanding>(OwningAbility);
}

void UAbilityTask_JumpAndWaitForLanding::Activate()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());

	if (!Character)
	{
		EndTask();
		return;
	}

	Character->Jump();
	Character->MovementModeChangedDelegate.AddDynamic(this, &UAbilityTask_JumpAndWaitForLanding::OnMovementModeChanged);
}

void UAbilityTask_JumpAndWaitForLanding::OnMovementModeChanged(
	ACharacter* Character,
	EMovementMode PrevMovementMode,
	uint8 PreviousCustomMode
)
{
	if (Character->GetCharacterMovement()->MovementMode != MOVE_Falling)
	{
		OnLanded.Broadcast();
		CleanAndEndTask();
	}
}

void UAbilityTask_JumpAndWaitForLanding::CleanAndEndTask()
{
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
	{
		Character->MovementModeChangedDelegate.RemoveDynamic(this, &UAbilityTask_JumpAndWaitForLanding::OnMovementModeChanged);
	}

	EndTask();
}
