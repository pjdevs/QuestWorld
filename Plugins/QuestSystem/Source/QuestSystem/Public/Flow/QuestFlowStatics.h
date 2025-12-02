// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UQuestDataAsset;
class UQuestSubsystem;
class UFlowNodeBase;

/**
 * 
 */
class QUESTSYSTEM_API FQuestFlowStatics
{
public:
	static UQuestSubsystem& GetQuestSubsystemFromFlowNodeBase(const UFlowNodeBase* FlowNodeBase);

#if WITH_EDITOR
public:
	static TSoftObjectPtr<UQuestDataAsset> GetOwningQuestAssetFromFlowNodeBase(const UFlowNodeBase* FlowNodeBase);
#endif
};
