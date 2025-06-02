// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Containers/Queue.h"
#include "NotificationSubsystem.generated.h"


class UBaseNotificationWidget;
struct FGameNotification;

/**
 * 
 */
UCLASS()
class NOTIFICATIONPLUGIN_API UNotificationSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void QueueNotification(const FGameNotification& Message);

	UFUNCTION(BlueprintCallable)
	void Subscribe(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable)
	void Unsubscribe(APlayerController* PlayerController);
	
	// void SaveToGame(USaveGame* SaveGame);
	// void LoadFromGame(USaveGame* SaveGame);
private:
	void TryDisplayNextNotification();
	void DisplayNotification(APlayerController* PlayerController, const FGameNotification& Notification);
	void OnNotificationEnded();
	
private:
	TQueue<FGameNotification> QueuedMessages;

	UPROPERTY()
	TArray<TWeakObjectPtr<APlayerController>> Subscribers;

	UPROPERTY()
	TObjectPtr<UBaseNotificationWidget> DisplayedWidget;

	bool bIsDisplaying;
};