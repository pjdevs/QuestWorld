// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_JumpAndWaitForLanding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLandedDelegate);

/**
 * 
 */
UCLASS()
class GAMEPLAYABILITYSYSTEMPLUGIN_API UAbilityTask_JumpAndWaitForLanding : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnLandedDelegate OnLanded;

	/** Start jump and wait for landing */
	UFUNCTION(
		BlueprintCallable,
		Category = "Ability|Tasks",
		meta = (
			DisplayName = "Jump And Wait For Landing",
			HidePin = "OwningAbility",
			DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "TRUE"
		)
	)
	static UAbilityTask_JumpAndWaitForLanding* JumpAndWaitForLanding(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;

private:
	UFUNCTION()
	void OnMovementModeChanged(ACharacter* Character, EMovementMode PrevMovementMode, uint8 PreviousCustomMode);

	void CleanAndEndTask();
};
