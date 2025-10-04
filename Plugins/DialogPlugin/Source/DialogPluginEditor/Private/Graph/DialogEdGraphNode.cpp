// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogEdGraphNode.h"

void UDialogEdGraphNode::AllocateDefaultPins()
{
	CreatePin(
		EGPD_Input,
		"MultipleNodes",
		FName(),
		nullptr,
		"Parent Dialog"
	);
	CreatePin(
		EGPD_Output,
		"MultipleNodes",
		FName(),
		nullptr,
		"Next Dialogs"
	);
}

FText UDialogEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Dialog Node");
}
