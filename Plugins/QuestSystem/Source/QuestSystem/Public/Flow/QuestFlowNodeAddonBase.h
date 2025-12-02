// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestId.h"
#include "AddOns/FlowNodeAddOn.h"
#include "QuestFlowNodeAddonBase.generated.h"

class UQuestDataAsset;
class UQuestSubsystem;

/**
 * 
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType, HideCategories = Object)
class QUESTSYSTEM_API UQuestFlowNodeAddonBase : public UFlowNodeAddOn
{
	GENERATED_BODY()

public:
	UQuestFlowNodeAddonBase();

protected:
	UQuestSubsystem& GetQuestSubsystem() const;
	FQuestId GetOwningQuestId() const;

#if WITH_EDITOR
protected:
	UFUNCTION()
	TSoftObjectPtr<UQuestDataAsset> GetOwningQuestAsset() const;
#endif
};
