// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogTrigger.h"
#include "GrantAbilitiesTrigger.generated.h"

class UGameplayAbility;

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UGrantAbilitiesTrigger : public UDialogTrigger
{
	GENERATED_BODY()

public:
	virtual void OnExecute_Implementation(AController* DialogController) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;
};
