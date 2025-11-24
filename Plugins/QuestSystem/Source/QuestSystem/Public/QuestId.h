// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestId.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestId
{
	GENERATED_BODY()
	
public:
	FQuestId();
	FQuestId(const FName& QuestName);

	bool IsValid() const;
	FString ToString() const { return QuestAssetId.ToString(); }

	bool operator==(const FQuestId& Other) const
	{
		return QuestAssetId == Other.QuestAssetId;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedTypes = Quest))
	FPrimaryAssetId QuestAssetId;
};

FORCEINLINE uint32 GetTypeHash(const FQuestId& QuestId) { return GetTypeHash(QuestId.QuestAssetId); }
