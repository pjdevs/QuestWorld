#include "QuestSystemEditor.h"

#include "QuestPhaseListCustomization.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModule"

void FQuestSystemEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout(
		"QuestPhaseList",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FQuestPhaseListCustomization::MakeInstance)
	);
}

void FQuestSystemEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FQuestSystemEditorModule, QuestSystemEditor)