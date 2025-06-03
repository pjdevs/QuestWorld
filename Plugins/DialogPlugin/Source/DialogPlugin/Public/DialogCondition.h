// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogCondition.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class DIALOGPLUGIN_API UDialogCondition : public UObject
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintImplementableEvent, Category = Dialog)
	virtual bool IsSatisfied(UWorld* World) { return true; }
};
