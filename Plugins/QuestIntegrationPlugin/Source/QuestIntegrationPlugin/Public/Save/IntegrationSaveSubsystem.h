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
	void LoadSaveGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void LoadSaveGameFromConfig();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void LoadGame();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void SaveGame();
	
private:
	UPROPERTY()
	UIntegrationSaveGame* SaveGameObject;
};
