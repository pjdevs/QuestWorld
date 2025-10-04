// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogEvents.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDialogEvents : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DIALOGPLUGIN_API IDialogEvents
{
	GENERATED_BODY()

public:
	virtual void OnDialogStarted(AController* DialogController) = 0;
	virtual void OnDialogEnded(AController* DialogController) = 0;
};
