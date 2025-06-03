// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogTrigger.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class DIALOGPLUGIN_API UDialogTrigger : public UObject
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintNativeEvent, Category = Dialog)
	virtual void Execute(UWorld* World);

// protected:
// 	virtual void Execute_Implementation()
// 	PURE_VIRTUAL(UDialogTrigger::Execute_Implementation, );
};
