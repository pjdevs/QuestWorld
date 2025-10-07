// Copyright pjdevs. All Rights Reserved.


#include "Graph/ChoiceDialogEdGraphNode.h"


UChoiceDialogEdGraphNode::UChoiceDialogEdGraphNode()
	: DialogChoices({ FText::FromString("Sample Choice") })
{
	AddChoiceAction = FUIAction(FExecuteAction::CreateLambda([this]
	{
		GetGraph()->Modify();

		DialogChoices.Add(FText::FromString("New Choice"));
		RebuildOutputPinsForChoices();

		GetGraph()->NotifyNodeChanged(this);
	}));
}

void UChoiceDialogEdGraphNode::RebuildOutputPinsForChoices()
{
	for (UEdGraphPin* ChoicePin : ChoicePins)
	{
		RemovePin(ChoicePin);
	}
	
	ChoicePins.Empty();
	
	for (auto&& DialogChoice : DialogChoices)
	{
		ChoicePins.Add(CreateDialogPin(EGPD_Output, FName(DialogChoice.ToString())));
	}
}

void UChoiceDialogEdGraphNode::AllocateDefaultPins()
{
	InputPin = CreateDialogPin(EGPD_Input,"Parent Dialog");

	RebuildOutputPinsForChoices();
}

void UChoiceDialogEdGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	FToolMenuSection& Section = Menu->AddSection(
		FName("ChoiceDialogActionsSection"),
		FText::FromString("Choice Dialog Actions")
	);

	Section.AddMenuEntry(
		FName("AddChoiceEntry"),
		FText::FromString("Add Choice"),
		FText::FromString("Add a dialog choice to this node"),
		FSlateIcon(),
		AddChoiceAction
	);
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
