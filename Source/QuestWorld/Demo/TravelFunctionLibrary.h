// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TravelFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class QUESTWORLD_API UTravelFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = World, meta=(WorldContext="WorldContextObject", AutoCreateRefTerm="Params"))
	static void ServerTravel(
		UObject* WorldContextObject,
		TSoftObjectPtr<UWorld> Level,
		const TMap<FString, FString>& Params,
		bool bShouldListen
	);
};
