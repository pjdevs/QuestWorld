// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

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

	static UDialogEdGraph* CreateEdGraphFromAsset(UDialogGraphAsset* DialogGraphAsset);
	static void CreateEdGraphNodesFromNode(
		UDialogEdGraph* DialogEdGraph,
		UDialogEdGraphNode* DialogParentGraphNode,
		UDialogNode* DialogNode,
		TMap<int, int>& MaxSiblingByDepth,
		int Depth = 0,
		int Sibling = 0,
		int ChoiceIndex = 0
	);

	// FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName("DialogGraphEditorApplication"); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString("DialogGraphEditorApplication"); }
	virtual FString GetWorldCentricTabPrefix() const override { return FString("DialogGraphEditorApplication"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::Red; }
	virtual FString GetDocumentationLink() const override { return FString("https://github.com/pjdevs/QuestWorld"); }
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override { }
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override { }

private:
	TObjectPtr<UDialogGraphAsset> WorkingAsset = nullptr;
	TObjectPtr<UDialogEdGraph> WorkingGraph = nullptr;
};
