// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogPluginEditor.h"
#include "IAssetTools.h"
#include "Graph/DialogGraphTypeActions.h"
#include "EdGraphUtilities.h"
#include "Graph/DialogGraphSlate.h"

#define LOCTEXT_NAMESPACE "FDialogPluginModule"

class UDialogEdGraph;

void FDialogPluginEditorModule::StartupModule()
{
	// Register custom dialog asset
	IAssetTools& AssetToolsModule = IAssetTools::Get();

	const EAssetTypeCategories::Type DialogAssetCategoryType = AssetToolsModule.RegisterAdvancedAssetCategory(
		FName("Dialog"),
		FText::FromString("Dialog")
	);

	const TSharedPtr<FDialogGraphTypeActions> DialogGraphAssetAction = MakeShareable(
		new FDialogGraphTypeActions(DialogAssetCategoryType)
	);
	AssetToolsModule.RegisterAssetTypeActions(DialogGraphAssetAction.ToSharedRef());

	// Make and register a dialog pin factory
	DialogPinFactory = MakeShareable(new FDialogGraphPanelPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(DialogPinFactory);

	// TODO Investigate custom visual
	// DialogNodeFactory = MakeShareable(new FDialogNodeFactory());
	// FEdGraphUtilities::RegisterVisualNodeFactory(DialogNodeFactory);
}

void FDialogPluginEditorModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogPluginEditorModule, DialogPluginEditor)