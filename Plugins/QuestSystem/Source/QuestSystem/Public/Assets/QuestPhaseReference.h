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

public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UQuestDataAsset> QuestRef;
#endif
	
	UPROPERTY(EditAnywhere)
	FQuestPhaseReferences Phases;

public:
	bool IsValid() const;
	
#if WITH_EDITOR
public:
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	virtual UQuestDataAsset* GetOwningQuest() const override;
#endif
};
