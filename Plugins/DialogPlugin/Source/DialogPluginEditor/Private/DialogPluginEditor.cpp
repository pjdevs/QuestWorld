// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogPluginEditor.h"

#include "DialogGraphAsset.h"
#include "IAssetTools.h"
#include "LevelEditor.h"
#include "Graph/DialogEdGraphNode.h"
#include "Graph/DialogGraphAssetEditor.h"
#include "Graph/DialogGraphTypeActions.h"

#define LOCTEXT_NAMESPACE "FDialogPluginModule"

class UDialogEdGraph;

void FDialogPluginEditorModule::StartupModule()
{
	// Add a test menu
	FLevelEditorModule& LevelEditorModule = 
		FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	const TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension(
		"WindowLayout",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateRaw(this, &FDialogPluginEditorModule::AddMenuEntry)
	);

	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);

	// Register custom actions types
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

void FDialogPluginEditorModule::AddMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString("Open Test Dialog Graph Editor"),
		FText::FromString("Opens a dummy dialog graph editor without an asset"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &FDialogPluginEditorModule::OpenTestGraphEditor))
	);
}

void FDialogPluginEditorModule::OpenTestGraphEditor()
{
	UDialogEdGraph* TempGraph = NewObject<UDialogEdGraph>(GetTransientPackage(), UDialogEdGraph::StaticClass());
	UDialogEdGraphNode* NewNode = NewObject<UDialogEdGraphNode>(TempGraph);
	NewNode->SetFlags(RF_Transactional);
	NewNode->NodePosX = 200;
	NewNode->NodePosY = 200;
	NewNode->AllocateDefaultPins();
	TempGraph->AddNode(NewNode);

	const TSharedRef<FDialogGraphAssetEditor> EditorToolkit = MakeShareable(new FDialogGraphAssetEditor());
	EditorToolkit->InitDialogGraphEditor(
		EToolkitMode::Standalone,
		TSharedPtr<IToolkitHost>(), 
		TempGraph
	);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogPluginEditorModule, DialogPluginEditor)