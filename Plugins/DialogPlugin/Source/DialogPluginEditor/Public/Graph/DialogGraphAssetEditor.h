// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "DialogEdGraph.h"
#include "Toolkits/AssetEditorToolkit.h"

class FDialogGraphAssetEditor : public FAssetEditorToolkit
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	
	TSharedRef<SDockTab> SpawnGraphEditorTab(const FSpawnTabArgs& Args);
	
	void InitDialogGraphEditor(
		const EToolkitMode::Type Mode,
		const TSharedPtr<IToolkitHost>& InitToolkitHost,
		UDialogEdGraph* DialogAsset
	);

protected:
	UDialogEdGraph* GraphBeingEdited = nullptr;
	TSharedPtr<SGraphEditor> GraphEditorWidget;
};