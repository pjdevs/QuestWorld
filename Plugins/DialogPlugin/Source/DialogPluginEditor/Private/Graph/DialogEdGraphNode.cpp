// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogEdGraphNode.h"

void UDialogEdGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, "MultipleNodes", FName(), nullptr, TEXT("In"));
	CreatePin(EGPD_Output, "MultipleNodes", FName(), nullptr, TEXT("Out"));
}

FText UDialogEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString("Dialog Node");
}
