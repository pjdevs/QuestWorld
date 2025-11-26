// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestOwnedObjectInterface.h"
#include "QuestPhaseList.h"
#include "UObject/Object.h"
#include "QuestPhaseReference.generated.h"

class UQuestDataAsset;

/**
 * 
 */
UCLASS(EditInlineNew, DefaultToInstanced)
class QUESTSYSTEM_API UQuestPhaseReference : public UObject, public IQuestOwnedObjectInterface
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, meta = (AllowNone = false))
	TSoftObjectPtr<UQuestDataAsset> Quest;
#endif
	
	UPROPERTY(EditAnywhere)
	FQuestPhaseList Phases;

#if WITH_EDITOR
public:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	
#if WITH_EDITOR
public:
	virtual UQuestDataAsset* GetOwningQuest() const override;
#endif
};
