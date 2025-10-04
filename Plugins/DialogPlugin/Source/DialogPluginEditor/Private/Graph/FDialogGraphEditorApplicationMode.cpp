// Copyright pjdevs. All Rights Reserved.


#include "Graph/FDialogGraphEditorApplicationMode.h"

#include "Graph/DialogGraphEditorTabFactory.h"


FDialogGraphEditorApplicationMode::FDialogGraphEditorApplicationMode(
	TSharedPtr<FDialogGraphEditorApplication> InOwnerApplication
)
	: FApplicationMode(GraphEditorModeName), OwnerApplication(InOwnerApplication)
{
	AllowedTabSet.RegisterFactory(MakeShareable(new FDialogGraphEditorTabFactory(InOwnerApplication)));

	TabLayout = FTabManager::NewLayout("DialogGraphEditorApplicationMode_Layout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Vertical)
				-> Split(
					FTabManager::NewStack()
						->AddTab(DialogGraphEditorTabName, ETabState::Type::OpenedTab)
				)
		);
}

void FDialogGraphEditorApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	const TSharedPtr<FDialogGraphEditorApplication> OwnedApplication = OwnerApplication.Pin();
	OwnedApplication->PushTabFactories(AllowedTabSet);

	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FDialogGraphEditorApplicationMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FDialogGraphEditorApplicationMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
