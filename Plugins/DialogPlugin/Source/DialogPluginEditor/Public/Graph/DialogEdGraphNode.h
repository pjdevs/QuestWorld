// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogEdGraphNode.generated.h"

const FName DialogPinSubCategory = FName("DialogPin");

/**
 * 
 */
UCLASS()
class DIALOGPLUGINEDITOR_API UDialogEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UDialogEdGraphNode();
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return GetLineText(); }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Red; }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual void AllocateDefaultPins() override;

	UEdGraphPin* CreateDialogPin(EEdGraphPinDirection PinDirection, FName PinName);
	UEdGraphPin* GetDialogInputPin() const { return InputPin; }
	UEdGraphPin* GetDialogOutputPin() const { return OutputPin; }

	const FText& GetLineText() const { return DialogLineText; }
	void SetLineText(const FText& LineText) { DialogLineText = LineText; }
	
private:
	UPROPERTY(EditAnywhere, Category = "Dialog", meta = (AllowPrivateAccess = true))
	FText DialogLineText;

private:
	FUIAction AddPinAction;
	FUIAction RemoveLastPinAction;
	FUIAction RemoveNodeAction;

	UEdGraphPin* InputPin;
	UEdGraphPin* OutputPin;
};
