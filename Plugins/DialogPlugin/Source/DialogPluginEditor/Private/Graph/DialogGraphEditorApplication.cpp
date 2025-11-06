// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphEditorApplication.h"
#include "ChoiceDialogNode.h"
#include "DialogChoice.h"
#include "DialogGraphAsset.h"
#include "DialogNode.h"
#include "SingleDialogNode.h"
#include "DialogTrigger.h"
#include "DialogCondition.h"
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

	if (!WorkingAsset)
	{
		return;
	}

	WorkingGraph = CreateEdGraphFromAsset(WorkingAsset);
	
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

	OnGraphChangedHandle = WorkingGraph->AddOnGraphChangedHandler(
		FOnGraphChanged::FDelegate::CreateSP(this, &FDialogGraphEditorApplication::OnGraphChanged)
	);
}

TObjectPtr<UDialogGraphAsset> FDialogGraphEditorApplication::GetWorkingAsset()
{
	return WorkingAsset;
}

TObjectPtr<UDialogEdGraph> FDialogGraphEditorApplication::GetWorkingGraph()
{
	return WorkingGraph;
}

void FDialogGraphEditorApplication::SetGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor)
{
	WorkingGraphEditor = InGraphEditor;
}

void FDialogGraphEditorApplication::SetSelectedNodeDetailsView(TSharedPtr<IDetailsView> InDetailsView)
{
	SelectedNodeDetailsView = InDetailsView;
	SelectedNodeDetailsView->OnFinishedChangingProperties().AddRaw(
		this,
		&FDialogGraphEditorApplication::OnNodeDetailsViewPropertiesChanged
	);
}

void FDialogGraphEditorApplication::OnClose()
{
	UpdateAssetFromEdGraph(WorkingAsset, WorkingGraph);
	WorkingGraph->RemoveOnGraphChangedHandler(OnGraphChangedHandle);
	
	FWorkflowCentricApplication::OnClose();
}

void FDialogGraphEditorApplication::OnGraphChanged(const FEdGraphEditAction& EditAction)
{
	UpdateAssetFromEdGraph(WorkingAsset, WorkingGraph);
}

void FDialogGraphEditorApplication::OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection)
{
	for (UObject* SelectedObject : Selection)
	{
		SelectedNodeDetailsView->SetObject(SelectedObject);
		return;
	}

	SelectedNodeDetailsView->SetObject(nullptr);
}

void FDialogGraphEditorApplication::OnNodeDetailsViewPropertiesChanged(const FPropertyChangedEvent& Event)
{
	if (WorkingGraphEditor == nullptr)
	{
		return;
	}

	for (UObject* SelectedObject : WorkingGraphEditor->GetSelectedNodes())
	{
		// TODO For now only choice node require pin update but make this better and more generic 
		if (UChoiceDialogEdGraphNode* ChoiceNode = Cast<UChoiceDialogEdGraphNode>(SelectedObject))
		{
			ChoiceNode->RebuildOutputPinsForChoices();
		}
	}

	WorkingGraphEditor->NotifyGraphChanged();
}

UDialogEdGraph* FDialogGraphEditorApplication::CreateEdGraphFromAsset(UDialogGraphAsset* DialogGraphAsset)
{
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
		SingleDialogParentNode->GetNextDialogPin()->MakeLinkTo(DialogEdGraphNode->GetDialogInputPin());
	}
	else if (const UChoiceDialogEdGraphNode* ChoiceDialogParentNode = Cast<UChoiceDialogEdGraphNode>(ParentDialogEdGraphNode))
	{
		UEdGraphPin* ParentPin = ChoiceDialogParentNode->GetNextDialogsPins()[ChoiceIndex]; 
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
	DialogGraphAsset->Modify();
}

void FDialogGraphEditorApplication::CreateAssetNodesFromEdNode(
	UDialogGraphAsset* DialogGraphAsset,
	UDialogNode* DialogNode,
	const UDialogEdGraphNode* DialogEdGraphNode
)
{
	if (USingleDialogNode* SingleDialogNode = Cast<USingleDialogNode>(DialogNode))
	{
		const USingleDialogEdGraphNode* SingleDialogEdGraphNode = Cast<USingleDialogEdGraphNode>(DialogEdGraphNode);

		if (const UDialogEdGraphNode* LinkedDialogEdNode = SingleDialogEdGraphNode->GetNextNode())
		{
			UDialogNode* NextDialogNode = CreateAssetNode(DialogGraphAsset, LinkedDialogEdNode);
			SingleDialogNode->SetNextDialog(NextDialogNode);

			CreateAssetNodesFromEdNode(DialogGraphAsset, NextDialogNode, LinkedDialogEdNode);
		}
	}
	else if (UChoiceDialogNode* ChoiceDialogNode = Cast<UChoiceDialogNode>(DialogNode))
	{
		const UChoiceDialogEdGraphNode* ChoiceDialogEdGraphNode = Cast<UChoiceDialogEdGraphNode>(DialogEdGraphNode);

		for (int i = 0; i < ChoiceDialogEdGraphNode->GetNextDialogsPins().Num(); i++)
		{
			if (const UDialogEdGraphNode* LinkedDialogEdNode = ChoiceDialogEdGraphNode->GetNextNodeForChoice(i))
			{
				UDialogNode* NextDialogNode = CreateAssetNode(DialogGraphAsset, LinkedDialogEdNode);
				ChoiceDialogNode->GetDialogChoices()[i]->SetNextDialog(NextDialogNode);
			
				CreateAssetNodesFromEdNode(DialogGraphAsset, NextDialogNode, LinkedDialogEdNode);
			}
		}
	}
}

UDialogNode* FDialogGraphEditorApplication::CreateAssetNode(
	UDialogGraphAsset* DialogGraphAsset,
	const UDialogEdGraphNode* DialogEdGraphNode
)
{
	UDialogNode* DialogNode;

	if (const USingleDialogEdGraphNode* SingleDialogEdGraphNode = Cast<USingleDialogEdGraphNode>(DialogEdGraphNode))
	{
		USingleDialogNode* SingleDialogNode = NewObject<USingleDialogNode>(DialogGraphAsset);
		SingleDialogNode->SetLine(SingleDialogEdGraphNode->DialogLineText);
		SingleDialogNode->SetNextDialog(nullptr);

		DialogNode = SingleDialogNode;
	}
	else if (const UChoiceDialogEdGraphNode* ChoiceDialogEdGraphNode = Cast<UChoiceDialogEdGraphNode>(DialogEdGraphNode))
	{
		UChoiceDialogNode* ChoiceDialogNode = NewObject<UChoiceDialogNode>(DialogGraphAsset);

		for (int i = 0; i < ChoiceDialogEdGraphNode->DialogChoices.Num(); i++)
		{
			UDialogChoice* Choice = NewObject<UDialogChoice>(ChoiceDialogNode);
			Choice->SetChoiceText(ChoiceDialogEdGraphNode->DialogChoices[i]);
			Choice->SetNextDialog(nullptr);
			ChoiceDialogNode->AddDialogChoice(Choice);
		}

		DialogNode = ChoiceDialogNode;
	}
	else
	{
		return nullptr;
	}
	
	DialogNode->EditorNodePosition = DialogEdGraphNode->GetPosition();

	// ! Duplicate objects with node as owner otherwise will be wiped on content cook
	
	for (auto&& Condition : DialogEdGraphNode->Conditions)
	{
		DialogNode->AddCondition(DuplicateObject(Condition, DialogNode));
	}
	
	for (auto&& Trigger : DialogEdGraphNode->Triggers)
	{
		DialogNode->AddTrigger(DuplicateObject(Trigger, DialogNode));
	}

	return DialogNode;
}
