// Copyright pjdevs. All Rights Reserved.


#include "Graph/FDialogGraphEditorApplicationMode.h"

#include "Graph/DialogGraphDetailsTabFactory.h"
#include "Graph/DialogGraphEditorTabFactory.h"


FDialogGraphEditorApplicationMode::FDialogGraphEditorApplicationMode(
	TSharedPtr<FDialogGraphEditorApplication> InOwnerApplication
)
	: FApplicationMode(GraphEditorModeName), OwnerApplication(InOwnerApplication)
{
	AllowedTabSet.RegisterFactory(MakeShareable(new FDialogGraphEditorTabFactory(InOwnerApplication)));
	AllowedTabSet.RegisterFactory(MakeShareable(new FDialogGraphDetailsTabFactory(InOwnerApplication)));

	TabLayout = FTabManager::NewLayout("DialogGraphEditorApplicationMode_Layout_v1")
		->AddArea(
			FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Vertical)
				-> Split(
					FTabManager::NewSplitter()
						->SetOrientation(Orient_Horizontal)
						->Split(
							FTabManager::NewStack()
								->SetSizeCoefficient(0.75)
								->AddTab(DialogGraphEditorTabName, ETabState::Type::OpenedTab)
						)
						->Split(
						FTabManager::NewStack()
								->SetSizeCoefficient(0.25)
								->AddTab(DialogGraphDetailsTabName, ETabState::Type::OpenedTab)
					)
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
