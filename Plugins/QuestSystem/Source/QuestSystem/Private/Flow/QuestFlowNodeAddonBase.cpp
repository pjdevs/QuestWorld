// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowNodeAddonBase.h"

#include "QuestSubsystem.h"
#include "Flow/QuestFlowAsset.h"
#include "Flow/QuestFlowStatics.h"


UQuestFlowNodeAddonBase::UQuestFlowNodeAddonBase()
{
#if WITH_EDITOR
	NodeDisplayStyle = FlowNodeStyle::AddOn_PerSpawnedActor;
	Category = TEXT("Quest");
#endif
}

UQuestSubsystem& UQuestFlowNodeAddonBase::GetQuestSubsystem() const
{
	return FQuestFlowStatics::GetQuestSubsystemFromFlowNodeBase(this);
}

FQuestId UQuestFlowNodeAddonBase::GetOwningQuestId() const
{
	UQuestFlowAsset* QuestFlowInstance = Cast<UQuestFlowAsset>(GetFlowAsset());
	ensureAlwaysMsgf(QuestFlowInstance != nullptr, TEXT("UQuestFlowNodeBase should be in a UQuestFlowAsset."));
	return GetQuestSubsystem().GetQuestIdFromFlow(QuestFlowInstance);
}

#if WITH_EDITOR
TSoftObjectPtr<UQuestDataAsset> UQuestFlowNodeAddonBase::GetOwningQuestAsset() const
{
	return FQuestFlowStatics::GetOwningQuestAssetFromFlowNodeBase(this);
}
#endif
