// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestPhaseReference.generated.h"

class UQuestDataAsset;

/**
 * 
 */
USTRUCT(BlueprintType)
struct QUESTSYSTEM_API FQuestPhaseReference
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UQuestDataAsset> QuestRef;
#endif
	
	UPROPERTY(EditAnywhere)
	FName Phase;

#if WITH_EDITOR
public:
	void FixupReference();
	bool IsValid() const;
#endif
};
