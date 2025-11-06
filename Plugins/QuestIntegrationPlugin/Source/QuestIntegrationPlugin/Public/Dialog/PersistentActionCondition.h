// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogCondition.h"
#include "PersistentActionCondition.generated.h"

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UPersistentActionCondition : public UDialogCondition
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied_Implementation(AController* DialogController) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	FName ActionToCheck;

	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	bool bInvert;
};
