// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPWorldStateSaveData.h"
#include "Subsystems/WorldSubsystem.h"
#include "IPWorldStateSaveSubsystem.generated.h"

/**
 * Subsystem to handle saving the state of savable actors.
 */
UCLASS()
class INTERACTIONPLUGIN_API UIPWorldStateSaveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;
	
public:
	void LoadSaveData(const FIPWorldStateSaveData& WorldSaveData);
	const FIPWorldStateSaveData& GetSaveData();

private:
	// void OnActorSpawned(AActor* Actor);

	UFUNCTION()
	void OnSavableActorDestroyed(AActor* DestroyedActor);

private:
	UPROPERTY()
	TArray<AActor*> SavableActorArray;

	FDelegateHandle OnActorSpawnedDelegateHandle;
	FIPWorldStateSaveData CurrentWorldSaveData;
};
