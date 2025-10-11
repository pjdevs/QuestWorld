// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestNotificationComponent.generated.h"


struct FQuestDescription;
class UQuestNotificationSettings;

UCLASS(ClassGroup=(Quest), meta=(BlueprintSpawnableComponent))
class QUESTINTEGRATIONPLUGIN_API UQuestNotificationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestNotificationComponent();

	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnQuestStarted(const FQuestDescription& Quest);

	UFUNCTION()
	void OnQuestCompleted(const FQuestDescription& Quest);

	UFUNCTION()
	void OnQuestUpdated(const FQuestDescription& Quest);

private:
	UPROPERTY(EditDefaultsOnly, Category = Quest, meta = (AllowPrivateAccess = true))
	TObjectPtr<UQuestNotificationSettings> QuestNotificationSettings;
};
