// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogCondition.h"
#include "QuestDialogCondition.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EQuestConditionBehavior : uint8
{
	QuestNotStartNorCompleted UMETA(DisplayName = "Quest Not Started Nor Completed"),
	QuestStarted UMETA(DisplayName = "Quest Started"),
	QuestCompleted UMETA(DisplayName = "Quest Completed"),
};

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UQuestDialogCondition : public UDialogCondition
{
	GENERATED_BODY()

public:
	virtual bool IsSatisfied(UWorld* World) override;	

private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog, meta = (AllowPrivateAccess = true, AllowedTypes = Quest))
	FPrimaryAssetId QuestId;

	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	EQuestConditionBehavior Behavior;
};
