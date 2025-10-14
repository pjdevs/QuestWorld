// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

// TODO wrap primary asset id in custom struct to allow select only quest asset id (c.f. inventory plugin) 

class FQuestSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
