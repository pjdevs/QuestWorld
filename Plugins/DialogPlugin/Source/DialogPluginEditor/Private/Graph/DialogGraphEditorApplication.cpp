// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphEditorApplication.h"

#include "DialogGraphAsset.h"
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
