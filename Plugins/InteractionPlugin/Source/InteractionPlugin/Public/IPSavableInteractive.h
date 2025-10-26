// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPSavableInteractive.generated.h"

struct FInteractiveSaveData;

// This class does not need to be modified.
UINTERFACE()
class UIPSavableInteractive : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONPLUGIN_API IIPSavableInteractive
{
	GENERATED_BODY()

public:
	virtual FName GetUniqueId() const = 0;
	virtual bool IsSavable() const = 0;
	virtual void LoadFromSave(const FInteractiveSaveData& InteractiveSaveData) = 0;
	virtual FInteractiveSaveData WriteToSave() = 0;
};
