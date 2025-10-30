// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPStateSavable.generated.h"

struct FIPStateSaveData;

// This class does not need to be modified.
UINTERFACE()
class UIPStateSavable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INTERACTIONPLUGIN_API IIPStateSavable
{
	GENERATED_BODY()

public:
	virtual FName GetUniqueId() const = 0;
	virtual bool IsSavable() const = 0;
	virtual void LoadFromSave(const FIPStateSaveData& SaveData) = 0;
	virtual FIPStateSaveData WriteToSave() = 0;
};
