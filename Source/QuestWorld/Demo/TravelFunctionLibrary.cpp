// Copyright pjdevs. All Rights Reserved.


#include "TravelFunctionLibrary.h"

void UTravelFunctionLibrary::ServerTravel(
	UObject* WorldContextObject,
	TSoftObjectPtr<UWorld> Level,
	const TMap<FString, FString>& Params,
	bool bShouldListen
)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);

	if (!World)
	{
		return;
	}

	const FSoftObjectPath Path = Level.ToSoftObjectPath();

	if (!Path.IsValid())
	{
		return;
	}
	
	const FString& LevelName = Path.GetAssetName();
	FString Url = LevelName;

	if (bShouldListen)
	{
		Url.Append("?listen");
	}

	if (!Params.IsEmpty())
	{
		FString ParamsString;
		
		for (const TPair<FString, FString>& Pair : Params)
		{
			ParamsString.Append(FString::Printf(TEXT("?%s=%s"), *Pair.Key, *Pair.Value));
		}

		Url.Append(ParamsString);
	}
	
	World->ServerTravel(Url, true);
}
