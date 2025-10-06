// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogEdGraphNode.h"
#include "Graph/PinCategories.h"

UDialogEdGraphNode::UDialogEdGraphNode()
{
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
		FText::FromString("Single Dialog Actions")
	);

	Section.AddMenuEntry(
		FName("RemoveNodeEntry"),
		FText::FromString("Remove Node"),
		FText::FromString("Remove the node from the graph"),
		FSlateIcon(),
		RemoveNodeAction
	);
}

UEdGraphPin* UDialogEdGraphNode::CreateDialogPin(EEdGraphPinDirection PinDirection, FName PinName)
{
	return CreatePin(
		PinDirection,
		FName("DialogFlow"),
		DialogPinSubCategory,
		nullptr,
		PinName
	);
}
