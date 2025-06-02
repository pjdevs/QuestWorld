// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameNotification.generated.h"

class UBaseNotificationWidget;

/**
 * 
 */
USTRUCT(BlueprintType)
struct NOTIFICATIONPLUGIN_API FGameNotification
{
	GENERATED_BODY()

	/** The class of notification widget to instantiate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UBaseNotificationWidget> NotificationWidgetClass;
	
	/** A tag or identifier to filter or route notifications */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NotificationCategory;

	/** Default message (can be used directly or overridden by payload) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Message;

	/** Optional priority. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Priority = 0;

	/** Optional priority. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = 0;
	
	/** Optional sound, icon, etc. if used in most messages */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> Icon = nullptr;
	
	/** Optional sound. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> Sound = nullptr;
	
	/** Pointer to a custom payload object for advanced use cases */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UObject> Payload = nullptr;
};
