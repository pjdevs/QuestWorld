// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogEdGraphSchema.h"

void UDialogEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const TSharedPtr<FEdGraphSchemaAction> NewNodeAction = MakeShareable(
		new FEdGraphSchemaAction(
			FText::FromString("Dialog"),
			FText::FromString("Dialog Node"),
			FText::FromString("Adds a dialog node"),
			0
		)
	);

	ContextMenuBuilder.AddAction(NewNodeAction);
}

void UDialogEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UDialogEdGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Connections not allowed (minimal schema)."));
}
