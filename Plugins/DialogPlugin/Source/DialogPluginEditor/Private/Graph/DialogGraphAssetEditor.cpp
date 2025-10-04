// Copyright pjdevs. All Rights Reserved.

#include "Graph/DialogGraphAssetEditor.h"

TSharedRef<SDockTab> FDialogGraphAssetEditor::SpawnGraphEditorTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.Label(FText::FromString("Dialog Graph"))
		[
			SNew(SBorder)
			.Padding(4)
			[
				SAssignNew(GraphEditorWidget, SGraphEditor)
					.GraphToEdit(GraphBeingEdited)
			]
		];
}

void FDialogGraphAssetEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
	InTabManager->RegisterTabSpawner(
		"GraphEditor",
		FOnSpawnTab::CreateSP(this, &FDialogGraphAssetEditor::SpawnGraphEditorTab)
	)
		.SetDisplayName(FText::FromString("Graph"))
		.SetGroup(WorkspaceMenuCategory.ToSharedRef());
}

void FDialogGraphAssetEditor::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
	InTabManager->UnregisterTabSpawner("GraphEditor");
}

FName FDialogGraphAssetEditor::GetToolkitFName() const
{
	return FName("Dialog Graph Editor");
}

FText FDialogGraphAssetEditor::GetBaseToolkitName() const
{
	return FText::FromString("Dialog Graph Editor");
}

FString FDialogGraphAssetEditor::GetWorldCentricTabPrefix() const
{
	return FString("DialogGraph");
}

FLinearColor FDialogGraphAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor::White;
}

void FDialogGraphAssetEditor::InitDialogGraphEditor(
	const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UDialogEdGraph* DialogGraph
)
{
	GraphBeingEdited = DialogGraph;

	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("DialogGraphEditorLayout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()
			->Split(
				FTabManager::NewStack()
				->AddTab("GraphEditor", ETabState::OpenedTab)
			)
		);

	InitAssetEditor(
		Mode,
		InitToolkitHost,
		TEXT("DialogGraphEditor"),
		Layout,
		true,
		true,
		DialogGraph
	);
}
