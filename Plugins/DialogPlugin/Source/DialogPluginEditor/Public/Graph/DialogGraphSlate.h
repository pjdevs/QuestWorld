// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "KismetPins/SGraphPinColor.h"
#include "KismetNodes/SGraphNodeK2Base.h"
#include "EdGraph/EdGraphPin.h"
#include "Graph/DialogEdGraphNode.h"

/**
 * 
 */
class SDialogGraphSlate : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SDialogGraphSlate) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
	}
	
protected:
	virtual FSlateColor GetPinColor() const override
	{
		return FSlateColor(FLinearColor::Red);
	}
};

struct FDialogGraphPanelPinFactory : public FGraphPanelPinFactory
{
	virtual ~FDialogGraphPanelPinFactory() override {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override
	{
		if (Pin->PinType.PinSubCategory == DialogPinSubCategory)
		{
			return SNew(SDialogGraphSlate, Pin);
		}

		return nullptr;
	}
};

class SDialogGraphNode : public SGraphNodeK2Base
{
public:
	SLATE_BEGIN_ARGS(SDialogGraphNode) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode* InNode)
	{
		GraphNode = InNode;
		SGraphNodeK2Base::Construct(SGraphNodeK2Base::FArguments());
	}

	virtual void UpdateGraphNode() override
	{
		SGraphNodeK2Base::UpdateGraphNode();

		// GetOrAddSlot(ENodeZone::Center)
		// 	.HAlign(HAlign_Center)
		// 	[
		// 		SAssignNew(DialogLineTextBlock, STextBlock)
		// 			.Text(FText::FromString("Super test text from start !"))
		// 			.Justification(ETextJustify::Center)
		// 			.ColorAndOpacity(FLinearColor::Yellow)
		// 	];
		//
		// if (const UDialogEdGraphNode* DialogEdGraphNode = Cast<UDialogEdGraphNode>(GraphNode))
		// {
		// 	DialogLineTextBlock->SetText(DialogEdGraphNode->GetLineText());
		// }
	}

private:
	TSharedPtr<STextBlock> DialogLineTextBlock = nullptr; // TOOD create
};

struct FDialogNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UDialogEdGraphNode* DialogNode = Cast<UDialogEdGraphNode>(Node))
		{
			return SNew(SDialogGraphNode, DialogNode);
		}

		return nullptr;
	}
};