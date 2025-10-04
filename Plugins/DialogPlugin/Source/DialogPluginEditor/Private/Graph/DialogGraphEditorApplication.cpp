// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphEditorApplication.h"

#include "DialogGraphAsset.h"
#include "Graph/DialogEdGraph.h"
#include "Graph/DialogEdGraphNode.h"
#include "Graph/DialogEdGraphSchema.h"
#include "Graph/FDialogGraphEditorApplicationMode.h"
#include "Kismet2/BlueprintEditorUtils.h"


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
	// TODO Make real conversion

	UDialogEdGraph* DialogGraph = Cast<UDialogEdGraph>(
		FBlueprintEditorUtils::CreateNewGraph(
			DialogGraphAsset,
			DialogGraphAsset->GetFName(),
			UDialogEdGraph::StaticClass(),
			UDialogEdGraphSchema::StaticClass()
		)
	);
	
	UDialogEdGraphNode* FirstNode = NewObject<UDialogEdGraphNode>(DialogGraph);
	FirstNode->NodePosX = 200;
	FirstNode->NodePosY = 200;
	FirstNode->AllocateDefaultPins();

	UDialogEdGraphNode* SecondNode = NewObject<UDialogEdGraphNode>(DialogGraph);
	SecondNode->NodePosX = 500;
	SecondNode->NodePosY = 200;
	SecondNode->AllocateDefaultPins();

	DialogGraph->AddNode(FirstNode);
	DialogGraph->AddNode(SecondNode);

	UEdGraphPin* FirstPin = FirstNode->FindPin(FName("Next Dialogs"));
	UEdGraphPin* SecondPin = SecondNode->FindPin(FName("Parent Dialog"));

	if (FirstPin && SecondPin)
	{
		FirstPin->MakeLinkTo(SecondPin);
	}

	return DialogGraph;
}
