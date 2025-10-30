// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogTrigger.h"
#include "PersistentActionTrigger.generated.h"

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UPersistentActionTrigger : public UDialogTrigger
{
	GENERATED_BODY()

public:
	virtual void OnExecute_Implementation(AController* DialogController) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	FName ActionToTrigger;
};
