// Copyright pjdevs. All Rights Reserved.


#include "Flow/QuestFlowNodeBase.h"

#include "QuestSubsystem.h"
#include "Flow/QuestFlowAsset.h"

UQuestFlowNodeBase::UQuestFlowNodeBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AllowedAssetClasses = { UQuestFlowAsset::StaticClass() };
}

UQuestSubsystem& UQuestFlowNodeBase::GetQuestSubsystem() const
{
	const FString& ThisClassName = this->GetClass()->GetFName().ToString();
	
	const UWorld* World = GetWorld();
	ensureMsgf(World != nullptr, TEXT("World is nullptr in node %s."), *ThisClassName);
	
	UQuestSubsystem* QuestSubsystem = World->GetGameInstance()->GetSubsystem<UQuestSubsystem>();
	ensureMsgf(QuestSubsystem != nullptr, TEXT("QuestSubsystem is nullptr in node %s."), *ThisClassName);

	return *QuestSubsystem;
}

FQuestId UQuestFlowNodeBase::GetOwningQuestId() const
{
	return GetQuestSubsystem().GetQuestIdFromFlow(GetFlowAsset());
}

void UQuestFlowNodeBase::OnOwningQuestChanged()
{
	
}

TSoftObjectPtr<UQuestDataAsset> UQuestFlowNodeBase::GetOwningQuestAsset() const
{
	const UQuestFlowAsset* OwningQuestFlowAsset = Cast<UQuestFlowAsset>(GetFlowAsset());
	ensureMsgf(
		OwningQuestFlowAsset != nullptr,
		TEXT("%s should be owned by a %s."),
		*this->GetClass()->GetFName().ToString(),
		*UQuestFlowAsset::StaticClass()->GetFName().ToString()
	);

	return OwningQuestFlowAsset->LinkedQuestRef;
}
