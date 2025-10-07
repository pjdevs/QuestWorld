// Copyright pjdevs. All Rights Reserved.


#include "Graph/SingleDialogEdGraphNode.h"


USingleDialogEdGraphNode::USingleDialogEdGraphNode()
	: DialogLineText(FText::FromString("Sample Dialog Line"))
{

}

FText USingleDialogEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (const FString& DialogLineString = DialogLineText.ToString(); DialogLineString.Len() > 20)
	{
		return FText::FromString(DialogLineString.Mid(0, 20).Append("..."));
	}

	return DialogLineText;
}

void USingleDialogEdGraphNode::AllocateDefaultPins()
{
	InputPin = CreateDialogPin(EGPD_Input,"Parent Dialog");
	OutputPin = CreateDialogPin(EGPD_Output,"Next Dialogs");
}

const UDialogEdGraphNode* USingleDialogEdGraphNode::GetNextNode() const
{
	if (OutputPin->LinkedTo.Num() > 0)
	{
		return Cast<UDialogEdGraphNode>(OutputPin->LinkedTo[0]->GetOwningNode());
	}
	
	return nullptr;
}
