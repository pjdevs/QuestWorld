// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FQuestSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
