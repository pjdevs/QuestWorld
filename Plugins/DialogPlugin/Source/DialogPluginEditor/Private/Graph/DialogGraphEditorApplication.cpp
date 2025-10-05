// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphEditorApplication.h"
#include "ChoiceDialogNode.h"
#include "DialogGraphAsset.h"
#include "DialogNode.h"
#include "SingleDialogNode.h"
#include "Graph/DialogEdGraph.h"
#include "Graph/DialogEdGraphNode.h"
#include "Graph/FDialogGraphEditorApplicationMode.h"


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
	CreateEdGraphNodesFromNode(DialogGraph, nullptr, DialogGraphAsset->GetDialogRoot(), MaxSiblingByDepth);

	return DialogGraph;
}

void FDialogGraphEditorApplication::CreateEdGraphNodesFromNode(
	UDialogEdGraph* DialogEdGraph,
	UDialogEdGraphNode* DialogParentGraphNode,
	UDialogNode* DialogNode,
	TMap<int, int>& MaxSiblingByDepth,
	int Depth,
	int Sibling,
	int ChoiceIndex
)
{
	const int MaxSiblingForDepth = MaxSiblingByDepth.FindOrAdd(Depth, Sibling);
	
	UDialogEdGraphNode* DialogEdGraphNode = NewObject<UDialogEdGraphNode>(DialogEdGraph);
	DialogEdGraphNode->CreateNewGuid();
	DialogEdGraphNode->SetPosition(FVector2f(Depth * 400.0f, MaxSiblingForDepth * 100.0f));
	DialogEdGraphNode->AllocateDefaultPins();

	if (const USingleDialogNode* SingleDialogNode = Cast<USingleDialogNode>(DialogNode))
	{
		DialogEdGraphNode->SetLineText(SingleDialogNode->GetLine());
	}
	else if (const UChoiceDialogNode* ChoiceDialogNode = Cast<UChoiceDialogNode>(DialogNode))
	{
		DialogEdGraphNode->SetLineText(ChoiceDialogNode->GetChoices()[ChoiceIndex]);
	}

	DialogEdGraph->AddNode(DialogEdGraphNode);

	if (DialogParentGraphNode != nullptr)
	{
		DialogParentGraphNode->GetDialogOutputPin()->MakeLinkTo(DialogEdGraphNode->GetDialogInputPin());
	}

	const TArray<TObjectPtr<UDialogNode>>& NextDialogs = DialogNode->GetNextDialogs();
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
