// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogEdGraphSchema.h"

#include "Graph/DialogEdGraph.h"
#include "Graph/DialogEdGraphNode.h"

void UDialogEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShareable(new FAddDialogNodeAction()));
}

const FPinConnectionResponse UDialogEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A == nullptr || B == nullptr)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "One of node is missing");
	}

	if (A->Direction == B->Direction)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, "Only output and inputs can be linked together");
	}

	if (B->Direction == EGPD_Input)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_B, "Make a new dialog link");
	}
	
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, "Make a new dialog link");
}

FAddDialogNodeAction::FAddDialogNodeAction()
	: FEdGraphSchemaAction(
		FText::FromString("Dialog"),
		FText::FromString("Dialog Node"),
		FText::FromString("Add a dialog node"),
		0
	)
{
}

UEdGraphNode* FAddDialogNodeAction::PerformAction(
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
	
	UDialogEdGraphNode* DialogNode = NewObject<UDialogEdGraphNode>(ParentGraph);
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
