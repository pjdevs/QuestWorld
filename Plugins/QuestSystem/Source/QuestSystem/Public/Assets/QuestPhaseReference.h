// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestOwnedObjectInterface.h"
#include "QuestPhaseReferences.h"
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
	UPROPERTY(EditAnywhere, Transient)
	TSoftObjectPtr<UQuestDataAsset> Quest;
#endif
	
	UPROPERTY(EditAnywhere)
	FQuestPhaseReferences Phases;

#if WITH_EDITOR
public:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif
	
#if WITH_EDITOR
public:
	virtual UQuestDataAsset* GetOwningQuest() const override;
#endif
};
