// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphDetailsTabFactory.h"
#include "Graph/DialogGraphEditorApplication.h"
#include "DialogGraphAsset.h"


FDialogGraphDetailsTabFactory::FDialogGraphDetailsTabFactory(TSharedPtr<FDialogGraphEditorApplication> InOwnerApplication)
	: FWorkflowTabFactory(DialogGraphDetailsTabName, InOwnerApplication), OwnerApplication(InOwnerApplication)
{
	TabLabel = FText::FromString("Dialog Details");
	ViewMenuDescription = FText::FromString("The Dialog Details View.");
	ViewMenuTooltip = FText::FromString("Show Dialog Details View.");
}

TSharedRef<SWidget> FDialogGraphDetailsTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
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

	const TSharedPtr<IDetailsView> GraphDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	GraphDetailsView->SetObject(Application->GetWorkingAsset());

	const TSharedPtr<IDetailsView> SelectedNodeDetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	SelectedNodeDetailsView->SetObject(nullptr);

	Application->SetSelectedNodeDetailsView(SelectedNodeDetailsView);
	
	return SNew(SVerticalBox)
		 + SVerticalBox::Slot()
			.FillHeight(0.5f)
			.HAlign(HAlign_Fill)
			[
				GraphDetailsView.ToSharedRef()
			]
		+ SVerticalBox::Slot()
		   .FillHeight(0.5f)
		   .HAlign(HAlign_Fill)
		   [
			   SelectedNodeDetailsView.ToSharedRef()
		   ];
}

FText FDialogGraphDetailsTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString("The dialog graph view for editing dialog graph.");
}
