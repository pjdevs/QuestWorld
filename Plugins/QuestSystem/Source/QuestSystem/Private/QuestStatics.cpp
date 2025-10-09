// Copyright pjdevs. All Rights Reserved.


#include "QuestStatics.h"
#include "GameFramework/GameStateBase.h"

void UQuestStatics::StartQuest(UObject* WorldContextObject, FPrimaryAssetId QuestId)
{
	if (!WorldContextObject)
	{
		return;
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (const AGameStateBase* GameState = World->GetGameState())
	{
		if (UQuestComponent* QuestComponent = GameState->FindComponentByClass<UQuestComponent>())
		{
			QuestComponent->StartQuest(QuestId);
		}
	}
}

TArray<FQuestDescription> UQuestStatics::GetActiveQuests(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return TArray<FQuestDescription>();
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (const AGameStateBase* GameState = World->GetGameState())
	{
		if (const UQuestComponent* QuestComponent = GameState->FindComponentByClass<UQuestComponent>())
		{
			return QuestComponent->GetActiveQuests();
		}
	}

	return TArray<FQuestDescription>();
}

TArray<FQuestDescription> UQuestStatics::GetCompletedQuests(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return TArray<FQuestDescription>();
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (const AGameStateBase* GameState = World->GetGameState())
	{
		if (const UQuestComponent* QuestComponent = GameState->FindComponentByClass<UQuestComponent>())
		{
			return QuestComponent->GetCompletedQuests();
		}
	}

	return TArray<FQuestDescription>();
}

bool UQuestStatics::IsQuestCompleted(UObject* WorldContextObject, FPrimaryAssetId QuestId)
{
	if (!WorldContextObject)
	{
		return false;
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (const AGameStateBase* GameState = World->GetGameState())
	{
		if (const UQuestComponent* QuestComponent = GameState->FindComponentByClass<UQuestComponent>())
		{
			return QuestComponent->IsQuestCompleted(QuestId);
		}
	}

	return false;
}

bool UQuestStatics::IsQuestActive(UObject* WorldContextObject, FPrimaryAssetId QuestId)
{
	if (!WorldContextObject)
	{
		return false;
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (const AGameStateBase* GameState = World->GetGameState())
	{
		if (const UQuestComponent* QuestComponent = GameState->FindComponentByClass<UQuestComponent>())
		{
			return QuestComponent->IsQuestActive(QuestId);
		}
	}

	return false;
}

void UQuestStatics::SubmitQuestEvent(UObject* WorldContextObject, UBaseQuestEvent* Event)
{
	if (!WorldContextObject)
	{
		return;
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (const AGameStateBase* GameState = World->GetGameState())
	{
		if (UQuestComponent* QuestComponent = GameState->FindComponentByClass<UQuestComponent>())
		{
			return QuestComponent->SubmitQuestEvent(Event);
		}
	}
}

UQuestComponent* UQuestStatics::GetQuestComponent(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);

	if (const AGameStateBase* GameState = World->GetGameState())
	{
		if (UQuestComponent* QuestComponent = GameState->FindComponentByClass<UQuestComponent>())
		{
			return QuestComponent;
		}
	}

	return nullptr;
}
