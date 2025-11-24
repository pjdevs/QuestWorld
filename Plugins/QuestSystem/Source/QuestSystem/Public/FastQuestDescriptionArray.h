// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestDescription.h"
#include "QuestId.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "UObject/Object.h"
#include "FastQuestDescriptionArray.generated.h"

DECLARE_DELEGATE_OneParam(FFastQuestDescriptionDelegate, const FQuestDescription&);

USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FFastQuestDescriptionEntry: public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY()
	FQuestDescription QuestDescription = FQuestDescription();
};

USTRUCT()
struct QUESTSYSTEM_API FFastQuestDescriptionArray : public FFastArraySerializer
{
	GENERATED_BODY()

public:
	FFastQuestDescriptionArray();

	void AddOrUpdateQuest(const FQuestDescription& QuestDescription);
	void RemoveQuest(const FQuestId& QuestId);
	TArray<FQuestDescription> GetQuests() const;

	FFastQuestDescriptionDelegate OnQuestAddedClient;
	FFastQuestDescriptionDelegate OnQuestRemovedClient;
	FFastQuestDescriptionDelegate OnQuestUpdatedClient;

public:
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);

private:
	UPROPERTY()
	TArray<FFastQuestDescriptionEntry> QuestDescriptions;
};

template<>
struct TStructOpsTypeTraits<FFastQuestDescriptionArray> : public TStructOpsTypeTraitsBase2<FFastQuestDescriptionArray>
{
	enum { WithNetDeltaSerializer = true };
};
