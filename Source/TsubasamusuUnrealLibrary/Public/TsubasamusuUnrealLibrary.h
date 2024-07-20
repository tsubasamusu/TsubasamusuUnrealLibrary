#pragma once

#include "Modules/ModuleManager.h"

class FTsubasamusuUnrealLibraryModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};