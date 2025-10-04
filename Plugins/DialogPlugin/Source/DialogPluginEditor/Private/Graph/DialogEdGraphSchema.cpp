// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogEdGraphSchema.h"

#include "Graph/DialogEdGraph.h"
#include "Graph/DialogEdGraphNode.h"

void UDialogEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	ContextMenuBuilder.AddAction(MakeShareable(new FAddDialogNodeAction()));
}

void UDialogEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UDialogEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
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
	DialogNode->NodePosX = Location.X;
	DialogNode->NodePosY = Location.Y;
	DialogNode->AllocateDefaultPins();

	DialogGraph->Modify();
	DialogGraph->AddNode(DialogNode, true, bSelectNewNode);

	return DialogNode;
}
