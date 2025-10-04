// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogPluginEditor.h"
#include "IAssetTools.h"
#include "Graph/DialogGraphTypeActions.h"

#define LOCTEXT_NAMESPACE "FDialogPluginModule"

class UDialogEdGraph;

void FDialogPluginEditorModule::StartupModule()
{
	IAssetTools& AssetToolsModule = IAssetTools::Get();

	const EAssetTypeCategories::Type DialogAssetCategoryType = AssetToolsModule.RegisterAdvancedAssetCategory(
		FName("Dialog"),
		FText::FromString("Dialog")
	);

	const TSharedPtr<FDialogGraphTypeActions> DialogGraphAssetAction = MakeShareable(
		new FDialogGraphTypeActions(DialogAssetCategoryType)
	);
	AssetToolsModule.RegisterAssetTypeActions(DialogGraphAssetAction.ToSharedRef());
}

void FDialogPluginEditorModule::ShutdownModule()
{
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogPluginEditorModule, DialogPluginEditor)