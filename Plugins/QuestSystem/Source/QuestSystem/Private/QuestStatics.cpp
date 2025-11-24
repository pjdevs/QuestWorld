// Copyright pjdevs. All Rights Reserved.


#include "QuestStatics.h"
#include "GameFramework/GameStateBase.h"


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
