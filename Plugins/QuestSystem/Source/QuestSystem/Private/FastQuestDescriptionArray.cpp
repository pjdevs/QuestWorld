// Copyright pjdevs. All Rights Reserved.


#include "FastQuestDescriptionArray.h"


FFastQuestDescriptionArray::FFastQuestDescriptionArray()
{
}

void FFastQuestDescriptionArray::AddOrUpdateQuest(const FQuestDescription& QuestDescription)
{
	FFastQuestDescriptionEntry* Existing = QuestDescriptions.FindByPredicate(
	[&](const FFastQuestDescriptionEntry& Item)
		{
			return Item.QuestDescription.QuestId == QuestDescription.QuestId;
		}
	);

	if (Existing)
	{
		Existing->QuestDescription = QuestDescription;
		MarkItemDirty(*Existing);
	}
	else
	{
		FFastQuestDescriptionEntry& NewQuestEntry = QuestDescriptions.AddDefaulted_GetRef();
		NewQuestEntry.QuestDescription = QuestDescription;
		MarkItemDirty(NewQuestEntry);
	}
}

void FFastQuestDescriptionArray::RemoveQuest(const FQuestId& QuestId)
{
	const int Index = QuestDescriptions.IndexOfByPredicate(
		[&](const FFastQuestDescriptionEntry& QuestEntry)
		{
			return QuestEntry.QuestDescription.QuestId == QuestId;
		}
	);

	if (Index != INDEX_NONE)
	{
		MarkItemDirty(QuestDescriptions[Index]);
		QuestDescriptions.RemoveAt(Index);
	}
}

TArray<FQuestDescription> FFastQuestDescriptionArray::GetQuests() const
{
	TArray<FQuestDescription> RawQuestDescriptions;
	RawQuestDescriptions.Reserve(QuestDescriptions.Num());
	
	for (const FFastQuestDescriptionEntry& QuestDescriptionEntry : QuestDescriptions)
	{
		RawQuestDescriptions.Add(QuestDescriptionEntry.QuestDescription);
	}
	
	return RawQuestDescriptions;
}

bool FFastQuestDescriptionArray::NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
{
	return FastArrayDeltaSerialize<FFastQuestDescriptionEntry, FFastQuestDescriptionArray>(QuestDescriptions, DeltaParms, *this);
}

void FFastQuestDescriptionArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (const int32 Idx : AddedIndices)
	{
		FFastQuestDescriptionEntry Entry = QuestDescriptions[Idx];
		OnQuestAddedClient.ExecuteIfBound(Entry.QuestDescription);
	}
}

void FFastQuestDescriptionArray::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (const int32 Idx : ChangedIndices)
	{
		const FQuestDescription& QuestDescription = QuestDescriptions[Idx].QuestDescription;
		OnQuestUpdatedClient.ExecuteIfBound(QuestDescription);
	}
}

void FFastQuestDescriptionArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (const int32 Idx : RemovedIndices)
	{
		const FQuestDescription& QuestDescription = QuestDescriptions[Idx].QuestDescription;
		OnQuestRemovedClient.ExecuteIfBound(QuestDescription);
	}
}