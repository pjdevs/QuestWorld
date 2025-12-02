#include "QuestSystemEditor.h"

#include "IAssetTools.h"
#include "QuestObjectiveReferenceCustomization.h"
#include "QuestPhaseReferencesCustomization.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModule"

void FQuestSystemEditorModule::StartupModule()
{
	// Register custom asset
	IAssetTools& AssetToolsModule = IAssetTools::Get();

	const EAssetTypeCategories::Type QuestFlowAssetCategoryType = AssetToolsModule.RegisterAdvancedAssetCategory(
		FName("Quest"),
		FText::FromString("Quest")
	);
	
	// Register custom property editors
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(
		"QuestPhaseReferences",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FQuestPhaseReferencesCustomization::MakeInstance)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		"QuestObjectiveReference",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FQuestObjectiveReferenceCustomization::MakeInstance)
	);
}

void FQuestSystemEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FQuestSystemEditorModule, QuestSystemEditor)