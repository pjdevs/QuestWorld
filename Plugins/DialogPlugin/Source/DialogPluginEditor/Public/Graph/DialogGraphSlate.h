// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "PinCategories.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"

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
		return GraphPinObj->GetOwningNode()->GetNodeTitleColor();
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

// class SDialogGraphNode : public SGraphNode
// {
// public:
// 	SLATE_BEGIN_ARGS(SDialogGraphNode) {}
// 	SLATE_END_ARGS()
//
// 	void Construct(const FArguments& InArgs, UEdGraphNode* InNode)
// 	{
// 		GraphNode = InNode;
// 		SGraphNode::Construct();
// 		UpdateGraphNode();
// 	}
//
// 	virtual void UpdateGraphNode() override
// 	{
// 		SGraphNode::UpdateGraphNode();
//
// 		LeftNodeBox->AddSlot()
// 			.HAlign(HAlign_Center)
// 			.VAlign(VAlign_Center)
// 			[
// 				SNew(STextBlock) //.Text(Cast<UDialogEdGraphNode>(GraphNode)->DialogLineText)
// 			];
// 	}
// };
//
// struct FDialogNodeFactory : public FGraphPanelNodeFactory
// {
// 	virtual TSharedPtr<SGraphNode> CreateNode(UEdGraphNode* Node) const override
// 	{
// 		if (UDialogEdGraphNode* DialogNode = Cast<UDialogEdGraphNode>(Node))
// 		{
// 			return SNew(SDialogGraphNode, DialogNode);
// 		}
//
// 		return nullptr;
// 	}
// };