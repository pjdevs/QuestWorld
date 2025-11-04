// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include "IPActionWidget.generated.h"

class UInputAction;

/**
 * Widget base class used to display an action inside rich text boxes.
 */
UCLASS(Abstract)
class INTERACTIONPLUGIN_API UIPActionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativePreConstruct() override;

	void SetAction(const UInputAction* InInputAction);
	
protected: 
	UFUNCTION(BlueprintImplementableEvent, Category = Input)
	void SetActionVisual(const UInputAction* InInputAction);

private:
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<const UInputAction> InputAction;
};
