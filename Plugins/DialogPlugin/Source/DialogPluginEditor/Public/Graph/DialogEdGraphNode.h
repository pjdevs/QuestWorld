// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogEdGraphNode.generated.h"

class UDialogTrigger;
class UDialogCondition;

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
	
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return DialogLineText; }
	virtual FLinearColor GetNodeTitleColor() const override { return FLinearColor::Red; }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual void AllocateDefaultPins() override;

	UEdGraphPin* CreateDialogPin(EEdGraphPinDirection PinDirection, FName PinName);
	UEdGraphPin* GetDialogInputPin() const { return InputPin; }
	UEdGraphPin* GetDialogOutputPin() const { return OutputPin; }


public:
	UPROPERTY(EditAnywhere, Category = "Dialog")
	FText DialogLineText;
	
	UPROPERTY(EditAnywhere, Category = "Dialog")
	TArray<TObjectPtr<UDialogCondition>> Conditions;

	UPROPERTY(EditAnywhere, Category = "Dialog")
	TArray<TObjectPtr<UDialogTrigger>> Triggers;
	
private:
	FUIAction AddPinAction;
	FUIAction RemoveLastPinAction;
	FUIAction RemoveNodeAction;

	UEdGraphPin* InputPin;
	UEdGraphPin* OutputPin;
};
