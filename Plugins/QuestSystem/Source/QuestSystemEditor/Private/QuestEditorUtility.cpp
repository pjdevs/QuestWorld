// Copyright pjdevs. All Rights Reserved.


#include "QuestEditorUtility.h"

#include "ClassViewerFilter.h"
#include "Assets/QuestObjective.h"
#include "Kismet2/SClassPickerDialog.h"

class FObjectiveClassFilter : public IClassViewerFilter
{
public:
	virtual bool IsClassAllowed(
		const FClassViewerInitializationOptions& InInitOptions,
		const UClass* InClass,
		TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs
	) override
	{
		return InClass->IsChildOf(UQuestObjective::StaticClass());
	}
	virtual bool IsUnloadedClassAllowed(
		const FClassViewerInitializationOptions& InInitOptions,
		const TSharedRef<const class IUnloadedBlueprintData> InUnloadedClassData,
		TSharedRef<class FClassViewerFilterFuncs> InFilterFuncs
	) override
	{
		return InUnloadedClassData->IsChildOf(UQuestObjective::StaticClass());
	}
};

UClass* UQuestEditorUtility::PickObjectiveClass()
{
	FClassViewerInitializationOptions InitOptions;
	InitOptions.Mode = EClassViewerMode::ClassPicker;
	InitOptions.bShowNoneOption = false;
	InitOptions.bShowUnloadedBlueprints = true;
	InitOptions.ClassFilters = { TSharedRef<FObjectiveClassFilter>(new FObjectiveClassFilter()) };

	UClass* ChosenClass = nullptr;
	const bool bPicked = SClassPickerDialog::PickClass(
		FText::FromString(TEXT("Select Objective Class")),
		InitOptions,
		ChosenClass,
		UQuestObjective::StaticClass()
	);

	return bPicked ? ChosenClass : nullptr;
}
