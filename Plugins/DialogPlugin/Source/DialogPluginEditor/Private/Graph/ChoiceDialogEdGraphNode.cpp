// Copyright pjdevs. All Rights Reserved.


#include "Graph/ChoiceDialogEdGraphNode.h"


UChoiceDialogEdGraphNode::UChoiceDialogEdGraphNode()
	: DialogChoices({ FText::FromString("Sample Choice") })
{

}

void UChoiceDialogEdGraphNode::AllocateDefaultPins()
{
	InputPin = CreateDialogPin(EGPD_Input,"Parent Dialog");

	ChoicePins.Empty();
	
	for (auto&& DialogChoice : DialogChoices)
	{
		ChoicePins.Add(CreateDialogPin(EGPD_Output, FName(DialogChoice.ToString())));
	}
}

const UDialogEdGraphNode* UChoiceDialogEdGraphNode::GetNextNodeForChoice(int ChoiceIndex) const
{
	if (ChoicePins.IsValidIndex(ChoiceIndex))
	{
		if (ChoicePins[ChoiceIndex]->LinkedTo.Num() > 0)
		{
			return Cast<UDialogEdGraphNode>(ChoicePins[ChoiceIndex]->LinkedTo[0]->GetOwningNode());
		}

	}

	return nullptr;
}
