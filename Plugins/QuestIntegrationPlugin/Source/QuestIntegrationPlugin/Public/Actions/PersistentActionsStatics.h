// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PersistentActionsStatics.generated.h"

class UPersistentActionsComponent;

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UPersistentActionsStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = PersitentActions, meta=(WorldContext="WorldContextObject"))
	static UPersistentActionsComponent* GetPersistentActions(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = PersitentActions, meta=(WorldContext="WorldContextObject"))
	static void SendPersistentAction(UObject* WorldContextObject, const FName& ActionName, AActor* ActionInstigator);

	UFUNCTION(BlueprintCallable, Category = PersitentActions, meta=(WorldContext="WorldContextObject"))
	static bool HasActionBeenDone(UObject* WorldContextObject, const FName& ActionName);
};
