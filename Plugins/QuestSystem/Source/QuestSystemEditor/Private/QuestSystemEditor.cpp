#include "QuestSystemEditor.h"

#include "IAssetTools.h"
#include "QuestObjectiveReferenceCustomization.h"
#include "QuestPhaseReferenceCustomization.h"
#include "QuestPhaseSetReferenceCustomization.h"
#include "Assets/QuestObjectiveReference.h"
#include "Assets/QuestPhaseReference.h"

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
		FQuestPhaseReference::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FQuestPhaseReferenceCustomization::MakeInstance)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FQuestPhaseSetReference::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FQuestPhaseSetReferenceCustomization::MakeInstance)
	);
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FQuestObjectiveReference::StaticStruct()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FQuestObjectiveReferenceCustomization::MakeInstance)
	);
}

void FQuestSystemEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FQuestSystemEditorModule, QuestSystemEditor)