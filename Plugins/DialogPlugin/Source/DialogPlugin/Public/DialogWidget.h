// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FDisplayLineFinishedDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDisplayChoicesFinishedDelegate, int, ChoiceIndex);

/**
 * 
 */
UCLASS()
class DIALOGPLUGIN_API UDialogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayLine(const FText& Text, const FDisplayLineFinishedDelegate& Delegate);

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayChoices(const TArray<FText>& Choices, const FDisplayChoicesFinishedDelegate& Delegate);
};
