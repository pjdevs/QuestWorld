// Copyright pjdevs. All Rights Reserved.


#include "DemoGameState.h"
#include "QuestComponent.h"


ADemoGameState::ADemoGameState()
{
	QuestComponent = CreateDefaultSubobject<UQuestComponent>("Quest Component");
}
