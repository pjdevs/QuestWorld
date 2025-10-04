// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphTypeActions.h"
#include "DialogGraphAsset.h"
#include "Graph/DialogEdGraphNode.h"
#include "Graph/DialogGraphAssetEditor.h"
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
			// UDialogEdGraph* DialogGraph = CreateEdGraphFromAsset(DialogGraphAsset);
			//
			// const TSharedRef<FDialogGraphAssetEditor> EditorToolkit = MakeShareable(new FDialogGraphAssetEditor());
			// EditorToolkit->InitDialogGraphEditor(
			// 	EToolkitMode::Standalone,
			// 	EditWithinLevelEditor, 
			// 	DialogGraph
			// );

			TSharedRef<FDialogGraphEditorApplication> Editor(new FDialogGraphEditorApplication());
			Editor->InitEditor(Mode, EditWithinLevelEditor, DialogGraphAsset);
		}
	}
}

uint32 FDialogGraphTypeActions::GetCategories()
{
	return CategoryType;
}

UDialogEdGraph* FDialogGraphTypeActions::CreateEdGraphFromAsset(UDialogGraphAsset* DialogGraphAsset)
{
	// TODO Make real conversion

	UDialogEdGraph* TempGraph = NewObject<UDialogEdGraph>(
		GetTransientPackage(),
		UDialogEdGraph::StaticClass()
	);
	UDialogEdGraphNode* NewNode = NewObject<UDialogEdGraphNode>(TempGraph);
	NewNode->SetFlags(RF_Transactional);
	NewNode->NodePosX = 200;
	NewNode->NodePosY = 200;
	NewNode->AllocateDefaultPins();
	TempGraph->AddNode(NewNode);

	return TempGraph;
}
