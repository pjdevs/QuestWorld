// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class QUESTSYSTEM_API FActiveQuest
{
public:
	explicit FActiveQuest(FName QuestId);
	~FActiveQuest();

	const FName& GetQuestId() const { return QuestId; }

private:
	FName QuestId;
};
