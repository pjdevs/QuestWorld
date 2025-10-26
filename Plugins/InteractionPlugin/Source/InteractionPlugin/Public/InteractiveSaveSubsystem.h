// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveWorldSaveData.h"
#include "Subsystems/WorldSubsystem.h"
#include "InteractiveSaveSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIONPLUGIN_API UInteractiveSaveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
public:
	void LoadSaveData(const FInteractiveWorldSaveData& WorldSaveData);
	const FInteractiveWorldSaveData& GetSaveData();

private:
	UFUNCTION()
	void OnSavableActorDestroyed(AActor* DestroyedActor);

private:
	UPROPERTY()
	TArray<AActor*> SavableActorArray;

	FInteractiveWorldSaveData CurrentWorldSaveData;
};
