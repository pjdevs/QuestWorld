// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphTypeActions.h"
#include "DialogGraphAsset.h"
#include "Graph/DialogGraphEditorApplication.h"

FDialogGraphTypeActions::FDialogGraphTypeActions(EAssetTypeCategories::Type InCategoryType)
	: CategoryType(InCategoryType)
{
}

FText FDialogGraphTypeActions::GetName() const
{
	return FText::FromString("Dialog Graph");
}

FColor FDialogGraphTypeActions::GetTypeColor() const
{
	return FColor::Red;
}

UClass* FDialogGraphTypeActions::GetSupportedClass() const
{
	return UDialogGraphAsset::StaticClass();
}

void FDialogGraphTypeActions::OpenAssetEditor(
	const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor
)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	
	for (auto&& Object : InObjects)
	{
		if (UDialogGraphAsset* DialogGraphAsset = Cast<UDialogGraphAsset>(Object))
		{
			const TSharedRef<FDialogGraphEditorApplication> Editor(new FDialogGraphEditorApplication());
			Editor->InitEditor(Mode, EditWithinLevelEditor, DialogGraphAsset);
		}
	}
}

uint32 FDialogGraphTypeActions::GetCategories()
{
	return CategoryType;
}
