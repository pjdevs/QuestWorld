// Copyright pjdevs. All Rights Reserved.


#include "QuestComponent.h"
#include "QuestSubsystem.h"
#include "Net/UnrealNetwork.h"


UQuestComponent::UQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	ActiveQuests.OnQuestAddedClient.BindLambda([&](const FQuestDescription& QuestDescription)
	{
		OnQuestStarted.Broadcast(QuestDescription);
	});
	ActiveQuests.OnQuestUpdatedClient.BindLambda([&](const FQuestDescription& QuestDescription)
	{
		OnQuestUpdated.Broadcast(QuestDescription);
	});

	CompletedQuests.OnQuestAddedClient.BindLambda([&](const FQuestDescription& QuestDescription)
	{
		OnQuestCompleted.Broadcast(QuestDescription);
	});
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() != ROLE_Authority)
	{
		if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
		{
			QuestSubsystem->OnQuestStarted.BindUObject(this, &UQuestComponent::OnQuestStartedServer);
			QuestSubsystem->OnQuestCompleted.BindUObject(this, &UQuestComponent::OnQuestCompletedServer);
			QuestSubsystem->OnQuestUpdated.BindUObject(this, &UQuestComponent::OnQuestUpdatedServer);

			for (const FQuestId& ActiveQuestId : QuestSubsystem->GetActiveQuests())
			{
				const FQuestDescription& QuestDescription = QuestSubsystem->GetQuestDescription(ActiveQuestId);
				ActiveQuests.AddOrUpdateQuest(QuestDescription);
			}

			for (const FQuestId& CompletedQuestId : QuestSubsystem->GetCompletedQuests())
			{
				const FQuestDescription& QuestDescription = QuestSubsystem->GetQuestDescription(CompletedQuestId);
				ActiveQuests.AddOrUpdateQuest(QuestDescription);
			}
		}
	}
}

void UQuestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UQuestComponent, ActiveQuests);
	DOREPLIFETIME(UQuestComponent, CompletedQuests);
}

TArray<FQuestDescription> UQuestComponent::GetActiveQuests() const
{
	return ActiveQuests.GetQuests();
}

TArray<FQuestDescription> UQuestComponent::GetCompletedQuests() const
{
	return CompletedQuests.GetQuests();
}

void UQuestComponent::OnQuestStartedServer(const FQuestId& StartedQuestId)
{
	if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
	{
		const FQuestDescription& QuestDescription = QuestSubsystem->GetQuestDescription(StartedQuestId);
		ActiveQuests.AddOrUpdateQuest(QuestDescription);
		OnQuestStarted.Broadcast(QuestDescription);
	}
}

void UQuestComponent::OnQuestCompletedServer(const FQuestId& CompletedQuestId)
{
	if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
	{
		ActiveQuests.RemoveQuest(CompletedQuestId);

		const FQuestDescription& QuestDescription = QuestSubsystem->GetQuestDescription(CompletedQuestId);
		CompletedQuests.AddOrUpdateQuest(QuestDescription);
	}
}

void UQuestComponent::OnQuestUpdatedServer(const FQuestId& UpdatedQuestId)
{
	if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
	{
		const FQuestDescription& QuestDescription = QuestSubsystem->GetQuestDescription(UpdatedQuestId);
		ActiveQuests.AddOrUpdateQuest(QuestDescription);
	}
}

void UQuestComponent::OnRep_ActiveQuests()
{
	bActiveQuestsReceived = true;
}

void UQuestComponent::OnRep_CompletedQuests()
{
	bCompletedQuestsReceived = true;
}
