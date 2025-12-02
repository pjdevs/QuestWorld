// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowAsset.h"

#include "QuestSubsystem.h"
#include "Flow/QuestFlowNodeBase.h"
#include "Nodes/Graph/FlowNode_Finish.h"

UQuestFlowAsset::UQuestFlowAsset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DeniedNodeClasses.Add(UFlowNode_Finish::StaticClass());
	ExpectedOwnerClass = UQuestSubsystem::StaticClass();
}

EDataValidationResult UQuestFlowAsset::ValidateAsset(FFlowMessageLog& MessageLog)
{
	if (!LinkedQuestRef.ToSoftObjectPath().IsValid())
	{
		MessageLog.Error(TEXT("LinkedQuestRef is not valid. Please reference a valid quest."), this);
		return EDataValidationResult::Invalid;
	}
	
	return Super::ValidateAsset(MessageLog);
}

void UQuestFlowAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UQuestFlowAsset, LinkedQuestRef))
	{
		for (const auto& [NodeGuid, Node] : GetNodes())
		{
			if (UQuestFlowNodeBase* QuestFlowNodeBase = Cast<UQuestFlowNodeBase>(Node))
			{
				QuestFlowNodeBase->OnOwningQuestChanged();
			}
		}
	}
}
