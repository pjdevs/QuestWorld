// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IntegrationSaveSubsystem.generated.h"

class UIntegrationSaveGame;
/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UIntegrationSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void LoadSaveGame();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void LoadGame();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void SaveGame();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void SetSaveGameSlotName(const FString& SlotName);
	
private:
	UPROPERTY()
	UIntegrationSaveGame* SaveGameObject;

	FString SaveGameSlotName = "QuestWorldSave";
};
