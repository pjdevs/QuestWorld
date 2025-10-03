// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogTrigger.h"
#include "ActionDialogTrigger.generated.h"

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UActionDialogTrigger : public UDialogTrigger
{
	GENERATED_BODY()

public:
	virtual void Execute(UWorld* World, AController* DialogController) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	FName ActionToTrigger;
};
