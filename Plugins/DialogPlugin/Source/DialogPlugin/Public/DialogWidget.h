// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogWidget.generated.h"

DECLARE_DELEGATE(FDisplayLineFinishedDelegate);
DECLARE_DELEGATE_OneParam(FDisplayChoicesFinishedDelegate, int);

/**
 * 
 */
UCLASS(Abstract)
class DIALOGPLUGIN_API UDialogWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetCharacterName(const FText& Text);
	
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayLine(const FText& Text);

	UFUNCTION(BlueprintImplementableEvent)
	void DisplayChoices(const TArray<FText>& Choices);

	UFUNCTION(BlueprintCallable)
	void DisplayLineFinished();
	
	UFUNCTION(BlueprintCallable)
	void DisplayChoicesFinished(int ChoiceIndex);
	
	FDisplayLineFinishedDelegate DisplayLineFinishedDelegate;

	FDisplayChoicesFinishedDelegate DisplayChoicesFinishedDelegate;
};
