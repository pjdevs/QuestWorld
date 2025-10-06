// Copyright pjdevs. All Rights Reserved.


#include "Graph/ChoiceDialogEdGraphNode.h"


UChoiceDialogEdGraphNode::UChoiceDialogEdGraphNode()
	: DialogChoices({ FText::FromString("Sample Choice") })
{

}

void UChoiceDialogEdGraphNode::AllocateDefaultPins()
{
	InputPin = CreateDialogPin(EGPD_Input,"Parent Dialog");

	OutputPins.Empty();
	
	for (auto&& DialogChoice : DialogChoices)
	{
		OutputPins.Add(CreateDialogPin(EGPD_Output, FName(DialogChoice.ToString())));
	}
}
