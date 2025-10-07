// Copyright pjdevs. All Rights Reserved.


#include "Graph/DialogGraphEditorTabFactory.h"
#include "Graph/DialogGraphEditorApplication.h"
#include "Graph/DialogEdGraph.h"


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

	SGraphEditor::FGraphEditorEvents GraphEvents;
	GraphEvents.OnSelectionChanged.BindRaw(
		Application.Get(),
		&FDialogGraphEditorApplication::OnGraphSelectionChanged
	);
	const TSharedPtr<SGraphEditor> GraphEditor =
		SNew(SGraphEditor)
			.IsEditable(true)
			.GraphEvents(GraphEvents)
			.GraphToEdit(Application->GetWorkingGraph());

	
	Application->SetGraphEditor(GraphEditor);
	
	return SNew(SVerticalBox)
		 + SVerticalBox::Slot()
			.FillHeight(1.0f)
			.HAlign(HAlign_Fill)
			[
				GraphEditor.ToSharedRef()
			];
}

FText FDialogGraphEditorTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString("The dialog graph view for editing dialog graph.");
}
