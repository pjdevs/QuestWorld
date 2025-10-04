// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "DialogGraphEditorApplication.h"

const FName GraphEditorModeName = FName("DialogGraphEditorApplicationMode");

/**
 * 
 */
class DIALOGPLUGINEDITOR_API FDialogGraphEditorApplicationMode : public FApplicationMode
{
public:
	FDialogGraphEditorApplicationMode(TSharedPtr<FDialogGraphEditorApplication> InOwnerApplication);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<FDialogGraphEditorApplication> OwnerApplication;
	FWorkflowAllowedTabSet AllowedTabSet;
};
