// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IPActionWidget.generated.h"

/**
 * Widget base class used to display an action inside rich text boxes.
 */
UCLASS(Abstract)
class INTERACTIONPLUGIN_API UIPActionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetAction(const FString& ActionName);
};
