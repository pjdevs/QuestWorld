// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogEdGraphSchema.h"

#include "Graph/ChoiceDialogEdGraphNode.h"
#include "Graph/DialogEdGraph.h"
#include "Graph/SingleDialogEdGraphNode.h"

void UDialogEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShareable(new FAddSingleDialogNodeAction()));
	ContextMenuBuilder.AddAction(MakeShareable(new FAddChoiceDialogNodeAction()));
}

const FPinConnectionResponse UDialogEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A == nullptr || B == nullptr)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "Need two node for a connection.");
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "Only inputs/outputs can be linked together.");
	}

	if (B->Direction == EGPD_Input)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_A, "Make new dialog link.");
	}
	else
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, "Make a new dialog link.");
	}
}

FAddSingleDialogNodeAction::FAddSingleDialogNodeAction()
	: FEdGraphSchemaAction(
		FText::FromString("Dialog"),
		FText::FromString("Single Dialog Node"),
		FText::FromString("Add a single dialog node"),
		0
	)
{
}

UEdGraphNode* FAddSingleDialogNodeAction::PerformAction(
	UEdGraph* ParentGraph,
	UEdGraphPin* FromPin,
	const FVector2f& Location,
	bool bSelectNewNode
)
{
	UDialogEdGraph* DialogGraph = Cast<UDialogEdGraph>(ParentGraph);

	if (!DialogGraph)
	{
		return nullptr;
	}
	
	USingleDialogEdGraphNode* DialogNode = NewObject<USingleDialogEdGraphNode>(ParentGraph);
	DialogNode->CreateNewGuid();
	DialogNode->NodePosX = Location.X;
	DialogNode->NodePosY = Location.Y;
	DialogNode->AllocateDefaultPins();

	if (FromPin != nullptr)
	{
		DialogNode->GetSchema()->TryCreateConnection(FromPin, DialogNode->GetDialogInputPin());
	}

	DialogGraph->Modify();
	DialogGraph->AddNode(DialogNode, true, bSelectNewNode);
	DialogGraph->NotifyGraphChanged();

	return DialogNode;
}

FAddChoiceDialogNodeAction::FAddChoiceDialogNodeAction()
	: FEdGraphSchemaAction(
		FText::FromString("Dialog"),
		FText::FromString("Choice Dialog Node"),
		FText::FromString("Add a choice dialog node"),
		0
	)
{
}

UEdGraphNode* FAddChoiceDialogNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2f& Location, bool bSelectNewNode)
{
	UDialogEdGraph* DialogGraph = Cast<UDialogEdGraph>(ParentGraph);

	if (!DialogGraph)
	{
		return nullptr;
	}
	
	UChoiceDialogEdGraphNode* DialogNode = NewObject<UChoiceDialogEdGraphNode>(ParentGraph);
	DialogNode->CreateNewGuid();
	DialogNode->NodePosX = Location.X;
	DialogNode->NodePosY = Location.Y;
	DialogNode->AllocateDefaultPins();

	if (FromPin != nullptr)
	{
		DialogNode->GetSchema()->TryCreateConnection(FromPin, DialogNode->GetDialogInputPin());
	}

	DialogGraph->Modify();
	DialogGraph->AddNode(DialogNode, true, bSelectNewNode);
	DialogGraph->NotifyGraphChanged();

	return DialogNode;
}
