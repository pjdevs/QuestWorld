// Copyright Epic Games, Inc. All Rights Reserved.

#include "DialogPluginEditor.h"
#include "LevelEditor.h"
#include "Graph/DialogEdGraphNode.h"
#include "Graph/DialogGraphAssetEditor.h"

#define LOCTEXT_NAMESPACE "FDialogPluginModule"

class UDialogEdGraph;

void FDialogPluginEditorModule::StartupModule()
{
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