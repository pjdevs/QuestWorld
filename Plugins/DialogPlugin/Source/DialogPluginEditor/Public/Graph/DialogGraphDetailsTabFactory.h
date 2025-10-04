// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

const FName DialogGraphDetailsTabName = FName("DialogGraphDetailsTab");

class FDialogGraphEditorApplication;

/**
 * 
 */
class DIALOGPLUGINEDITOR_API FDialogGraphDetailsTabFactory : public FWorkflowTabFactory
{
public:
	FDialogGraphDetailsTabFactory(TSharedPtr<FDialogGraphEditorApplication> InOwnerApplication);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

private:
	TWeakPtr<FDialogGraphEditorApplication> OwnerApplication;
};
