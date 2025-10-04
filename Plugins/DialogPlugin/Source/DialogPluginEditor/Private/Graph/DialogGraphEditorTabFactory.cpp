// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphEditorTabFactory.h"
#include "Graph/DialogGraphEditorApplication.h"
#include "DialogGraphAsset.h"


FDialogGraphEditorTabFactory::FDialogGraphEditorTabFactory(TSharedPtr<FDialogGraphEditorApplication> InOwnerApplication)
	: FWorkflowTabFactory(DialogGraphEditorTabName, InOwnerApplication), OwnerApplication(InOwnerApplication)
{
	TabLabel = FText::FromString("Dialog Graph");
	ViewMenuDescription = FText::FromString("The Dialog Graph View.");
	ViewMenuTooltip = FText::FromString("Show Dialog Graph View.");
}

TSharedRef<SWidget> FDialogGraphEditorTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	const TSharedPtr<FDialogGraphEditorApplication> Application = OwnerApplication.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowSearch = false;
		DetailsViewArgs.bHideSelectionTip = true;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bSearchInitialKeyFocus = true;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.NotifyHook = nullptr;
		DetailsViewArgs.bShowOptions = true;
		DetailsViewArgs.bShowModifiedPropertiesOption = false;
		DetailsViewArgs.bShowScrollBar = false;
	}

	const TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	DetailsView->SetObject(Application->GetWorkingAsset());

	return SNew(SVerticalBox)
		 +  SVerticalBox::Slot()
			.FillHeight(1.0f)
			.HAlign(HAlign_Fill)
			[
				DetailsView.ToSharedRef()	
			];
}

FText FDialogGraphEditorTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString("The dialog graph view for editing dialog graph.");
}
