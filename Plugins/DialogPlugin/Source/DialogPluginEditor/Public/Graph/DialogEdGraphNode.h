// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "DialogEdGraphNode.generated.h"

class UDialogTrigger;
class UDialogCondition;

/**
 * 
 */
UCLASS(Abstract)
class DIALOGPLUGINEDITOR_API UDialogEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UDialogEdGraphNode();

	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	UEdGraphPin* CreateDialogPin(EEdGraphPinDirection PinDirection, FName PinName);

	virtual UEdGraphPin* GetDialogInputPin() const PURE_VIRTUAL(UDialogEdGraphNode::GetDialogInputPin, return nullptr;); 

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Dialog")
	TArray<TObjectPtr<UDialogCondition>> Conditions;

	UPROPERTY(EditAnywhere, Instanced, Category = "Dialog")
	TArray<TObjectPtr<UDialogTrigger>> Triggers;
	
private:
	FUIAction RemoveNodeAction;
};
