// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogTrigger.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGPLUGIN_API UDialogTrigger : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = Dialog)
	void Execute();

protected:
	virtual void Execute_Implementation()
	PURE_VIRTUAL(UDialogTrigger::Execute_Implementation, );
};
