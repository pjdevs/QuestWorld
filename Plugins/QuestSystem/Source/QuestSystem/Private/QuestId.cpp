// Copyright pjdevs. All Rights Reserved.


#include "QuestId.h"
#include "PrimaryAssetTypes.h"


FQuestId::FQuestId()
	: FQuestId(NAME_None)
{
}

FQuestId::FQuestId(const FName& QuestName)
	: QuestAssetId(PrimaryAssetTypes::Quest, QuestName)
{
		
}

bool FQuestId::IsValid() const
{
	return QuestAssetId.IsValid() && QuestAssetId.PrimaryAssetType == PrimaryAssetTypes::Quest;
}

