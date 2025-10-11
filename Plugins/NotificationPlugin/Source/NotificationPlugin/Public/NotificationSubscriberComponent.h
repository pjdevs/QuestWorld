// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NotificationSubscriberComponent.generated.h"


UCLASS(ClassGroup=(Notification), meta=(BlueprintSpawnableComponent))
class NOTIFICATIONPLUGIN_API UNotificationSubscriberComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNotificationSubscriberComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
