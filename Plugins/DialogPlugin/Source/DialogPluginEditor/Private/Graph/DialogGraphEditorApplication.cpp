// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphEditorApplication.h"
#include "ChoiceDialogNode.h"
#include "DialogChoice.h"
#include "DialogGraphAsset.h"
#include "DialogNode.h"
#include "SingleDialogNode.h"
#include "Graph/DialogEdGraph.h"
#include "Graph/DialogEdGraphNode.h"
#include "EdGraph/EdGraphNode.h"
#include "Graph/ChoiceDialogEdGraphNode.h"
#include "Graph/FDialogGraphEditorApplicationMode.h"
#include "Graph/SingleDialogEdGraphNode.h"


void FDialogGraphEditorApplication::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FDialogGraphEditorApplication::InitEditor(
	const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject
)
{
	WorkingAsset = Cast<UDialogGraphAsset>(InObject);
	WorkingGraph = CreateEdGraphFromAsset(WorkingAsset);

	if (!WorkingAsset)
	{
		return;
	}
	
	const TArray ObjectsToEdit = { InObject };

	InitAssetEditor(
		Mode,
		InitToolkitHost,
		"DialogGraphEditor",
		FTabManager::FLayout::NullLayout,
		true,
		true,
		ObjectsToEdit
	);

	AddApplicationMode(
		GraphEditorModeName,
		MakeShareable(new FDialogGraphEditorApplicationMode(SharedThis(this)))
	);

	SetCurrentMode(GraphEditorModeName);
}

TObjectPtr<UDialogGraphAsset> FDialogGraphEditorApplication::GetWorkingAsset()
{
	return WorkingAsset;
}

TObjectPtr<UDialogEdGraph> FDialogGraphEditorApplication::GetWorkingGraph()
{
	return WorkingGraph;
}

UDialogEdGraph* FDialogGraphEditorApplication::CreateEdGraphFromAsset(UDialogGraphAsset* DialogGraphAsset)
{
	// UDialogEdGraph* DialogGraph = Cast<UDialogEdGraph>(
	// 	FBlueprintEditorUtils::CreateNewGraph(
	// 		DialogGraphAsset,
	// 		DialogGraphAsset->GetFName(),
	// 		UDialogEdGraph::StaticClass(),
	// 		UDialogEdGraphSchema::StaticClass()
	// 	)
	// );

	UDialogEdGraph* DialogGraph = NewObject<UDialogEdGraph>(DialogGraphAsset);

	TMap<int, int> MaxSiblingByDepth;
	CreateEdGraphNodesFromNode(
		DialogGraph,
		nullptr,
		DialogGraphAsset->GetDialogRoot(),
		MaxSiblingByDepth
	);

	return DialogGraph;
}

void FDialogGraphEditorApplication::CreateEdGraphNodesFromNode(
	UDialogEdGraph* DialogEdGraph,
	UDialogEdGraphNode* ParentDialogEdGraphNode,
	UDialogNode* DialogNode,
	TMap<int, int>& MaxSiblingByDepth,
	int Depth,
	int Sibling,
	int ChoiceIndex
)
{
	const int MaxSiblingForDepth = MaxSiblingByDepth.FindOrAdd(Depth, Sibling);
	TArray<UDialogNode*> NextDialogs;
	UDialogEdGraphNode* DialogEdGraphNode = nullptr;
	
	if (const USingleDialogNode* SingleDialogNode = Cast<USingleDialogNode>(DialogNode))
	{
		USingleDialogEdGraphNode* SingleDialogEdGraphNode = NewObject<USingleDialogEdGraphNode>(DialogEdGraph);
		SingleDialogEdGraphNode->DialogLineText = SingleDialogNode->GetLine();

		NextDialogs.Add(SingleDialogNode->GetNextDialog());
		DialogEdGraphNode = SingleDialogEdGraphNode;
	}
	else if (const UChoiceDialogNode* ChoiceDialogNode = Cast<UChoiceDialogNode>(DialogNode))
	{
		UChoiceDialogEdGraphNode* ChoiceDialogEdGraphNode = NewObject<UChoiceDialogEdGraphNode>(DialogEdGraph);
		ChoiceDialogEdGraphNode->DialogChoices.Empty();
		
		for (const TObjectPtr<UDialogChoice>& Choice : ChoiceDialogNode->GetDialogChoices())
		{
			ChoiceDialogEdGraphNode->DialogChoices.Add(Choice->GetChoiceText());
			NextDialogs.Add(Choice->GetNextDialog());	
		}
		DialogEdGraphNode = ChoiceDialogEdGraphNode;
	}
	else
	{
		return;
	}

	if (DialogNode->EditorNodePosition != FVector2f())
	{
		DialogEdGraphNode->SetPosition(DialogNode->EditorNodePosition);
	}
	else
	{
		DialogEdGraphNode->SetPosition(FVector2f(Depth * 400.0f, MaxSiblingForDepth * 100.0f));
	}
	
	DialogEdGraphNode->Conditions = DialogNode->GetConditions();
	DialogEdGraphNode->Triggers = DialogNode->GetTriggers();
	DialogEdGraphNode->CreateNewGuid();
	DialogEdGraphNode->AllocateDefaultPins();

	if (const USingleDialogEdGraphNode* SingleDialogParentNode = Cast<USingleDialogEdGraphNode>(ParentDialogEdGraphNode))
	{
		SingleDialogParentNode->GetDialogOutputPin()->MakeLinkTo(DialogEdGraphNode->GetDialogInputPin());
	}
	else if (const UChoiceDialogEdGraphNode* ChoiceDialogParentNode = Cast<UChoiceDialogEdGraphNode>(ParentDialogEdGraphNode))
	{
		UEdGraphPin* ParentPin = ChoiceDialogParentNode->GetDialogOutputPins()[ChoiceIndex]; 
		ParentPin->MakeLinkTo(DialogEdGraphNode->GetDialogInputPin());
	}

	DialogEdGraph->AddNode(DialogEdGraphNode);

	for (int i = 0; i < NextDialogs.Num(); i++)
	{
		const TObjectPtr<UDialogNode>& NextDialogNode = NextDialogs[i];
		
		const int NextSibling = Sibling + i;
		const int NextDepth = Depth + 1;
		int& MaxSiblingForNextDepth = MaxSiblingByDepth.FindOrAdd(NextDepth, NextSibling);
		++MaxSiblingForNextDepth;

		CreateEdGraphNodesFromNode(
			DialogEdGraph,
			DialogEdGraphNode,
			NextDialogNode,
			MaxSiblingByDepth,
			NextDepth,
			NextSibling,
			i
		);
	}
}

void FDialogGraphEditorApplication::UpdateAssetFromEdGraph(
	UDialogGraphAsset* DialogGraphAsset,
	UDialogEdGraph* DialogEdGraph
)
{
	// find root node
	const TObjectPtr<UEdGraphNode>* RootNode = DialogEdGraph->Nodes.FindByPredicate([](const TObjectPtr<UEdGraphNode>& Node)
	{
		if (const UDialogEdGraphNode* DialogNode = Cast<UDialogEdGraphNode>(Node))
		{
			if (!DialogNode->GetDialogInputPin()->HasAnyConnections())
			{
				return true;
			}
		}

		return false;
	});

	if (RootNode == nullptr)
	{
		return;
	}

	// Create child nodes
	const UDialogEdGraphNode* RootDialogEdGraphNode = Cast<UDialogEdGraphNode>(*RootNode);
	UDialogNode* RootDialogNode = CreateAssetNode(DialogGraphAsset, RootDialogEdGraphNode);

	CreateAssetNodesFromEdNode(DialogGraphAsset, RootDialogNode, RootDialogEdGraphNode);

	DialogGraphAsset->SetDialogRoot(RootDialogNode);
}

void FDialogGraphEditorApplication::CreateAssetNodesFromEdNode(
	UDialogGraphAsset* DialogGraphAsset,
	UDialogNode* DialogNode,
	const UDialogEdGraphNode* DialogEdGraphNode
)
{
	// for (UEdGraphPin*& ChildLinkedPin : DialogEdGraphNode->GetDialogOutputPin()->LinkedTo)
	// {
	// 	const UDialogEdGraphNode* ChildDialogEdGraphNode = Cast<UDialogEdGraphNode>(ChildLinkedPin->GetOwningNode());
	// 	UDialogNode* ChildDialogNode = CreateAssetNode(DialogGraphAsset, ChildDialogEdGraphNode);
	//
	// 	DialogNode->AddNextDialog(ChildDialogNode);
	// 	
	// 	CreateAssetNodesFromEdNode(DialogGraphAsset, ChildDialogNode, ChildDialogEdGraphNode);
	// }
}

UDialogNode* FDialogGraphEditorApplication::CreateAssetNode(
	UDialogGraphAsset* DialogGraphAsset,
	const UDialogEdGraphNode* DialogEdGraphNode
)
{
	UDialogNode* DialogNode = nullptr;
	// TArray<UEdGraphPin*>& ChildLinkedPins = DialogEdGraphNode->GetDialogOutputPin()->LinkedTo;
	//
	// if (ChildLinkedPins.Num() > 1)
	// {
	// 	USingleDialogNode* SingleDialogNode = NewObject<USingleDialogNode>(DialogGraphAsset);
	// 	SingleDialogNode->SetLine(DialogEdGraphNode->DialogLineText);
	//
	// 	DialogNode = SingleDialogNode;
	// }
	// else
	// {
	// 	TArray<FText> Choices;
	//
	// 	for ()
	// 	
	// 	UChoiceDialogNode* ChoiceDialogNode = NewObject<UChoiceDialogNode>(DialogGraphAsset);
	// 	ChoiceDialogNode->SetChoices(Choices);
	//
	// 	DialogNode = ChoiceDialogNode;
	// }
	//
	// DialogNode->EditorNodePosition = DialogEdGraphNode->GetPosition();
	//
	// for (auto&& Condition : DialogNode->GetConditions())
	// {
	// 	DialogNode->AddCondition(Condition);
	// }
	//
	// for (auto&& Trigger : DialogNode->GetTriggers())
	// {
	// 	DialogNode->AddTrigger(Trigger);
	// }

	return DialogNode;
}
