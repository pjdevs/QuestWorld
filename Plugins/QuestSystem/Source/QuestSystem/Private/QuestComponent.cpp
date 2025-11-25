// Copyright pjdevs. All Rights Reserved.


#include "QuestComponent.h"
#include "QuestSubsystem.h"
#include "Net/UnrealNetwork.h"


UQuestComponent::UQuestComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	KnownQuests.OnQuestAddedClient.BindLambda([&](const FQuestDescription& QuestDescription)
	{
		if (bKnownQuestsReceived)
		{
			OnQuestStarted.Broadcast(QuestDescription);
		}
	});
	KnownQuests.OnQuestUpdatedClient.BindLambda([&](const FQuestDescription& QuestDescription)
	{
		if (bKnownQuestsReceived)
		{
			if (QuestDescription.bIsCompleted)
			{
				OnQuestCompleted.Broadcast(QuestDescription);
			}
			else
			{
				OnQuestUpdated.Broadcast(QuestDescription);
			}
		}
	});
}

void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
		{
			QuestSubsystem->OnQuestStarted.BindUObject(this, &UQuestComponent::OnQuestUpdatedServer);
			QuestSubsystem->OnQuestCompleted.BindUObject(this, &UQuestComponent::OnQuestUpdatedServer);
			QuestSubsystem->OnQuestUpdated.BindUObject(this, &UQuestComponent::OnQuestUpdatedServer);

			for (const FQuestId& ActiveQuestId : QuestSubsystem->GetKnownQuests())
			{
				const FQuestDescription& QuestDescription = QuestSubsystem->GetQuestDescription(ActiveQuestId);
				KnownQuests.AddOrUpdateQuest(QuestDescription);
			}
		}
	}
}

void UQuestComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetOwnerRole() == ROLE_Authority)
	{
		if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
		{
			QuestSubsystem->OnQuestStarted.Unbind();
			QuestSubsystem->OnQuestCompleted.Unbind();
			QuestSubsystem->OnQuestUpdated.Unbind();
		}
	}
}

void UQuestComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UQuestComponent, KnownQuests);
}

TArray<FQuestDescription> UQuestComponent::GetKnownQuests() const
{
	return KnownQuests.GetQuests();
}

void UQuestComponent::OnQuestUpdatedServer(const FQuestId& UpdatedQuestId)
{
	if (UQuestSubsystem* QuestSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UQuestSubsystem>())
	{
		const FQuestDescription& QuestDescription = QuestSubsystem->GetQuestDescription(UpdatedQuestId);
		KnownQuests.AddOrUpdateQuest(QuestDescription);

		OnQuestUpdated.Broadcast(QuestDescription);
	}
}

void UQuestComponent::OnRep_KnownQuests()
{
	bKnownQuestsReceived = true;
}

