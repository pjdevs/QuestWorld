#include "Actions/PersistentActionsStatics.h"

#include "QuestStatics.h"
#include "Actions/PersistentActionsComponent.h"
#include "Assets/ActionQuestEvent.h"
#include "GameFramework/GameStateBase.h"

UPersistentActionsComponent* UPersistentActionsStatics::GetPersistentActions(UObject* WorldContextObject)
{
	const UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UPersistentActionsComponent* PersistentActionsComponent = World->GetGameState()->FindComponentByClass<UPersistentActionsComponent>();

	return PersistentActionsComponent;
}

void UPersistentActionsStatics::SendPersistentAction(
	UObject* WorldContextObject,
	const FName& ActionName,
	AActor* ActionInstigator
)
{
	if (UPersistentActionsComponent* PersistentActions = GetPersistentActions(WorldContextObject))
	{
		PersistentActions->AddActionDone(ActionName);

		if (UQuestComponent* QuestComponent = UQuestStatics::GetQuestComponent(WorldContextObject))
		{
			UActionQuestEvent* Event = NewObject<UActionQuestEvent>();
			Event->ActionName = ActionName;
			Event->ActionInstigator = ActionInstigator;

			QuestComponent->SubmitQuestEvent(Event);
		}
	}
}

bool UPersistentActionsStatics::HasActionBeenDone(UObject* WorldContextObject, const FName& ActionName)
{
	if (UPersistentActionsComponent* PersistentActions = GetPersistentActions(WorldContextObject))
	{
		return PersistentActions->GetActionsDone().Contains(ActionName);
	}

	return false;
}
