// Copyright pjdevs. All Rights Reserved.


#include "Flow/FlowNode_Quest.h"

#include "PrimaryAssetTypes.h"
#include "QuestStatics.h"

static const FName StartPinName(TEXT("Start"));
static const FName CompletePinName(TEXT("Complete"));
static const FName FailPinName(TEXT("Fail"));

UFlowNode_Quest::UFlowNode_Quest(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
#if WITH_EDITOR
	Category = TEXT("Quest");
#endif
	InputPins = { FFlowPin(StartPinName), FFlowPin(CompletePinName), FFlowPin(FailPinName) };
}

void UFlowNode_Quest::ExecuteInput(const FName& PinName)
{
	UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in FlowNode_Quest"));
	
	UQuestComponent* QuestComponent = UQuestStatics::GetQuestComponent(World);

	if (!QuestComponent)
	{
		return;
	}

	if (PinName == StartPinName)
	{
		
	}
	if (PinName == CompletePinName)
	{

	}
	else if (PinName == FailPinName)
	{
		
	}
}

#if WITH_EDITOR
FString UFlowNode_Quest::GetNodeDescription() const
{
	return QuestId.PrimaryAssetName.ToString();
}

EDataValidationResult UFlowNode_Quest::ValidateNode()
{
	if (!QuestId.IsValid() || QuestId.PrimaryAssetType != PrimaryAssetTypes::Quest)
	{
		ValidationLog.Error<UFlowNode>(TEXT("QuestId is invalid or not referencing a quest."), this);
		return EDataValidationResult::Invalid;
	}

	return EDataValidationResult::Valid;
}
#endif