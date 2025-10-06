// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogChoice.generated.h"

class UDialogNode;
/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class DIALOGPLUGIN_API UDialogChoice : public UObject
{
	GENERATED_BODY()

public:
	const FText& GetChoiceText() const { return ChoiceText; }
	void SetChoiceText(const FText& InChoiceText) { ChoiceText = InChoiceText; }

	UDialogNode* GetNextDialog() const { return NextDialog; }
	void SetNextDialog(UDialogNode* InNextDialog) { NextDialog = InNextDialog; }

private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog, meta = (AllowPrivateAccess = true))
	FText ChoiceText;

	UPROPERTY(EditDefaultsOnly, Instanced, Category = Dialog, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDialogNode> NextDialog;
};
