// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

const FName DialogGraphEditorTabName = FName("DialogGraphTab");

class FDialogGraphEditorApplication;

/**
 * 
 */
class DIALOGPLUGINEDITOR_API FDialogGraphEditorTabFactory : public FWorkflowTabFactory
{
public:
	FDialogGraphEditorTabFactory(TSharedPtr<FDialogGraphEditorApplication> InOwnerApplication);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<FDialogGraphEditorApplication> OwnerApplication;
};
