// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION(BlueprintImplementableEvent, Category = Input)
	void SetAction(const UInputAction* ActionName);

	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InputAction;
};
