// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "DialogEdGraphSchema.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGPLUGINEDITOR_API UDialogEdGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
};

USTRUCT()
struct FAddSingleDialogNodeAction : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FAddSingleDialogNodeAction();

	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2f& Location,
		bool bSelectNewNode = true
	) override;
};

USTRUCT()
struct FAddChoiceDialogNodeAction : public FEdGraphSchemaAction
{
	GENERATED_BODY()

public:
	FAddChoiceDialogNodeAction();

	virtual UEdGraphNode* PerformAction(
		UEdGraph* ParentGraph,
		UEdGraphPin* FromPin,
		const FVector2f& Location,
		bool bSelectNewNode = true
	) override;
};