// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Graph/DialogGraphSlate.h"
#include "Modules/ModuleManager.h"

struct FDialogGraphPanelPinFactory;

class FDialogPluginEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FDialogGraphPanelPinFactory> DialogPinFactory;
	TSharedPtr<FDialogNodeFactory> DialogNodeFactory;
};
