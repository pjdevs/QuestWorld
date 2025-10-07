// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class UChoiceDialogEdGraphNode;
class USingleDialogEdGraphNode;
class UDialogEdGraphNode;
class UDialogNode;
class UDialogEdGraph;
class UDialogGraphAsset;

/**
 * 
 */
class DIALOGPLUGINEDITOR_API FDialogGraphEditorApplication
	: public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook 
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	void InitEditor(
		const EToolkitMode::Type Mode,
		const TSharedPtr<IToolkitHost>& InitToolkitHost,
		UObject* InObject
	);

	TObjectPtr<UDialogGraphAsset> GetWorkingAsset();
	TObjectPtr<UDialogEdGraph> GetWorkingGraph();

	void SetGraphEditor(TSharedPtr<SGraphEditor> InGraphEditor);
	void SetSelectedNodeDetailsView(TSharedPtr<IDetailsView> InDetailsView);

	void OnGraphChanged(const FEdGraphEditAction& EditAction);
	void OnGraphSelectionChanged(const FGraphPanelSelectionSet& Selection);
	void OnNodeDetailsViewPropertiesChanged(const FPropertyChangedEvent& Event);
	
public: // FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName("DialogGraphEditorApplication"); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString("DialogGraphEditorApplication"); }
	virtual FString GetWorldCentricTabPrefix() const override { return FString("DialogGraphEditorApplication"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::Red; }
	virtual FString GetDocumentationLink() const override { return FString("https://github.com/pjdevs/QuestWorld"); }
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override { }
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override { }

	virtual void OnClose() override;

private: // Asset/EdGraph conversion helpers
	static UDialogEdGraph* CreateEdGraphFromAsset(UDialogGraphAsset* DialogGraphAsset);
	static void CreateEdGraphNodesFromNode(
		UDialogEdGraph* DialogEdGraph,
		UDialogEdGraphNode* ParentDialogEdGraphNode,
		UDialogNode* DialogNode,
		TMap<int, int>& MaxSiblingByDepth,
		int Depth = 0,
		int Sibling = 0,
		int ChoiceIndex = 0
	);

	static void UpdateAssetFromEdGraph(UDialogGraphAsset* DialogGraphAsset, UDialogEdGraph* DialogEdGraph);
	static void CreateAssetNodesFromEdNode(
		UDialogGraphAsset* DialogGraphAsset,
		UDialogNode* DialogNode,
		const UDialogEdGraphNode* DialogEdGraphNode
	);
	static UDialogNode* CreateAssetNode(
		UDialogGraphAsset* DialogGraphAsset,
		const UDialogEdGraphNode* DialogEdGraphNode
	);
	
private:
	TObjectPtr<UDialogGraphAsset> WorkingAsset = nullptr;
	TObjectPtr<UDialogEdGraph> WorkingGraph = nullptr;

	// TODO copied arch from tuto but the mode should handle that stuff not entire app
	TSharedPtr<SGraphEditor> WorkingGraphEditor = nullptr;
	TSharedPtr<IDetailsView> SelectedNodeDetailsView = nullptr;

	FDelegateHandle OnGraphChangedHandle;
};
