#include "QuestSystemEditor.h"

#include "QuestObjectiveReferenceCustomization.h"
#include "QuestPhaseListCustomization.h"

#define LOCTEXT_NAMESPACE "FQuestSystemEditorModule"

void FQuestSystemEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomPropertyTypeLayout(
		"QuestPhaseList",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FQuestPhaseListCustomization::MakeInstance)
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