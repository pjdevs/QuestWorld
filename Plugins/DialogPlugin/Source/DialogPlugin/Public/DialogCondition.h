// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogCondition.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGPLUGIN_API UDialogCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = Dialog)
	bool IsSatisfied();

protected:
	virtual bool IsSatisfied_Implementation()
	PURE_VIRTUAL(UDialogCondition::IsSatisfied_Implementation, return false;)
};
