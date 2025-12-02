// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FlowAsset.h"
#include "QuestId.h"
#include "QuestFlowAsset.generated.h"

/**
 * Flow graph based quest graph used to represent quest logic.
 */
UCLASS(BlueprintType, HideCategories = Object)
class QUESTSYSTEM_API UQuestFlowAsset : public UFlowAsset
{
	GENERATED_BODY()

public:
	UQuestFlowAsset(const FObjectInitializer& ObjectInitializer);
	
	virtual EDataValidationResult ValidateAsset(FFlowMessageLog& MessageLog) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
#if WITH_EDITORONLY_DATA
public:
	UPROPERTY(EditAnywhere, Category = "Quest")
	TSoftObjectPtr<UQuestDataAsset> LinkedQuestRef;
#endif
};
