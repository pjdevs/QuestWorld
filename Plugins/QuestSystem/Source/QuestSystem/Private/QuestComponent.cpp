// Copyright pjdevs. All Rights Reserved.


#include "QuestComponent.h"
#include "QuestService.h"
#include "Net/UnrealNetwork.h"


UQuestComponent::UQuestComponent()
	: QuestService(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UQuestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UQuestComponent, ActiveQuests);
}

void UQuestComponent::StartQuest(FPrimaryAssetId QuestId)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	QuestService->StartQuest(QuestId, GetWorld());
}

TArray<FQuestDescription> UQuestComponent::GetActiveQuests() const
{
	return ActiveQuests;
}

TArray<FQuestDescription> UQuestComponent::GetCompletedQuests() const
{
	return CompletedQuests;
}

bool UQuestComponent::IsQuestCompleted(FPrimaryAssetId QuestId) const
{
	return CompletedQuests.ContainsByPredicate(
		[&QuestId](const FQuestDescription& Quest)
		{
			return Quest.QuestId == QuestId;		
		}
	);
}

bool UQuestComponent::IsQuestActive(FPrimaryAssetId QuestId) const
{
	return ActiveQuests.ContainsByPredicate(
		[&QuestId](const FQuestDescription& Quest)
		{
			return Quest.QuestId == QuestId;		
		}
	);
}

void UQuestComponent::SubmitQuestEvent(UBaseQuestEvent* Event)
{
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	QuestService->SubmitQuestEvent(GetWorld(), Event);
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		InitQuestService();
	}
}

void UQuestComponent::InitQuestService()
{
	QuestService = NewObject<UQuestServiceImpl>(this, FName("QuestService"));
	QuestService->SetQuestStartedDelegate(FQuestEventDelegate::CreateUObject(this, &UQuestComponent::OnQuestStartedServer));
	QuestService->SetQuestCompletedDelegate(FQuestEventDelegate::CreateUObject(this, &UQuestComponent::OnQuestCompletedServer));
	QuestService->SetQuestUpdatedDelegate(FQuestEventDelegate::CreateUObject(this, &UQuestComponent::OnQuestUpdatedServer));
	QuestService->LoadQuests(FQuestLoadedDelegate::CreateUObject(this, &UQuestComponent::OnQuestsLoadedServer));
}

void UQuestComponent::OnQuestsLoadedServer()
{
	ActiveQuests = QuestService->GetActiveQuestDescriptions();
	CompletedQuests = QuestService->GetCompletedQuestDescriptions();
}

void UQuestComponent::OnQuestStartedServer(const FQuestDescription& StartedQuest)
{
	ActiveQuests = QuestService->GetActiveQuestDescriptions();

	Multicast_QuestStarted(StartedQuest);
}

void UQuestComponent::OnQuestCompletedServer(const FQuestDescription& CompletedQuest)
{
	ActiveQuests = QuestService->GetActiveQuestDescriptions();
	CompletedQuests = QuestService->GetCompletedQuestDescriptions();

	Multicast_QuestCompleted(CompletedQuest);
}

void UQuestComponent::OnQuestUpdatedServer(const FQuestDescription& UpdatedQuest)
{
	ActiveQuests = QuestService->GetActiveQuestDescriptions();

	Multicast_QuestUpdated(UpdatedQuest);
}

void UQuestComponent::Multicast_QuestCompleted_Implementation(const FQuestDescription& StartedQuest)
{
	OnQuestCompleted.Broadcast(StartedQuest);
}

void UQuestComponent::Multicast_QuestStarted_Implementation(const FQuestDescription& StartedQuest)
{
	OnQuestStarted.Broadcast(StartedQuest);
}

void UQuestComponent::Multicast_QuestUpdated_Implementation(const FQuestDescription& StartedQuest)
{
	OnQuestUpdated.Broadcast(StartedQuest);
}

void UQuestComponent::OnRep_ActiveQuests()
{
	OnActiveQuestsUpdated.Broadcast();	
}

void UQuestComponent::OnRep_CompletedQuests()
{
	OnCompletedQuestsUpdated.Broadcast();
}
