// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogEdGraphNode.h"

UDialogEdGraphNode::UDialogEdGraphNode()
{
	AddPinAction = FUIAction(FExecuteAction::CreateLambda([this]
	{
		GetGraph()->Modify();
		CreateDialogPin(EGPD_Output,"Another Next Dialog");
		GetGraph()->NotifyNodeChanged(this);
	}));

	RemoveLastPinAction = FUIAction(FExecuteAction::CreateLambda([this]
	{
		if (Pins.Num() <= 2)
		{
			return;
		}
		
		UEdGraphPin* Pin = GetPinAt(Pins.Num() - 1);

		if (!Pin || Pin->Direction == EGPD_Input)
		{
			return;
		}

		GetGraph()->Modify();
		RemovePin(Pin);
		GetGraph()->NotifyNodeChanged(this);
	}));

	RemoveNodeAction = FUIAction(FExecuteAction::CreateLambda([this]
	{
		GetGraph()->Modify();
		GetGraph()->RemoveNode(this);
		GetGraph()->NotifyGraphChanged();
	}));
}

void UDialogEdGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(
		FName("DialogActionsSection"),
		FText::FromString("Dialog Actions")
	);

	Section.AddMenuEntry(
		FName("AddPinEntry"),
		 FText::FromString("Add Pin"),
		FText::FromString("Add a pin to the node"),
		FSlateIcon(),
		AddPinAction
	);

	Section.AddMenuEntry(
		FName("RemoveLastPinEntry"),
		FText::FromString("Remove Last Pin"),
		FText::FromString("Remove last pin from the node"),
		FSlateIcon(),
		RemoveLastPinAction
);

	Section.AddMenuEntry(
		FName("RemoveNodeEntry"),
		FText::FromString("Remove Node"),
		FText::FromString("Remove the node from the graph"),
		FSlateIcon(),
		RemoveNodeAction
	);
}

void UDialogEdGraphNode::AllocateDefaultPins()
{
	InputPin = CreateDialogPin(EGPD_Input,"Parent Dialog");
	OutputPin = CreateDialogPin(EGPD_Output,"Next Dialogs");
}

UEdGraphPin* UDialogEdGraphNode::CreateDialogPin(EEdGraphPinDirection PinDirection, FName PinName)
{
	return CreatePin(
		PinDirection,
		FName("DialogFlow"), // PinDirection == EGPD_Input ? FName("Inputs") : FName("Outputs"),
		DialogPinSubCategory,
		nullptr,
		PinName
	);
}
