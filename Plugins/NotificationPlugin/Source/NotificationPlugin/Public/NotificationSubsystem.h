// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISpudObject.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NotificationSubsystem.generated.h"


class UBaseNotificationWidget;
struct FGameNotification;

// TODO work on this as it does not really support multiple local players
// because bIsDisplaying would lock other players etc.

/**
 * 
 */
UCLASS(Category = Notification)
class NOTIFICATIONPLUGIN_API UNotificationSubsystem
	: public UGameInstanceSubsystem, public ISpudObject, public ISpudObjectCallback
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public: // Spud
	virtual void SpudPostRestore_Implementation(const USpudState* State) override;
	
public:
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = Notification)
	void QueueNotification(const FGameNotification& Message);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = Notification)
	void Subscribe(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = Notification)
	void Unsubscribe(APlayerController* PlayerController);

private:
	void TryDisplayNextNotification();
	void DisplayNotification(APlayerController* PlayerController, const FGameNotification& Notification);
	void OnNotificationEnded();
	void OnNotificationDestroyed();

private:
	UPROPERTY(SaveGame)
	TArray<FGameNotification> QueuedMessages;

	TSet<TWeakObjectPtr<APlayerController>> Subscribers;

	UPROPERTY()
	TObjectPtr<UBaseNotificationWidget> DisplayedWidget;

	bool bIsDisplaying;
};