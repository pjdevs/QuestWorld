// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IntegrationSaveSubsystem.generated.h"

class UUIntegrationPlayerSaveGame;
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

	// Player save, for now will say world is in slot 0 and players ion the others
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void LoadPlayerSaveGame(const FString& SlotName, int PlayerIndex);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void LoadPlayerSaveGameFromConfig(int PlayerIndex);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void LoadPlayer(APlayerState* PlayerState, int PlayerIndex);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void SavePlayerSaveGame(const APlayerState* PlayerState, int PlayerIndex);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void SavePlayers();

	// Helper function
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save, meta = (WorldContext = "WorldContextObject"))
	void SaveAll(UObject* WorldContextObject);

private:
	static FString GetPlayerSlotName(const FString& SlotName, int PlayerIndex);
	
private:
	UPROPERTY()
	TObjectPtr<UIntegrationSaveGame> SaveGameObject;

	UPROPERTY()
	TMap<int, TObjectPtr<UUIntegrationPlayerSaveGame>> PlayerSaveGameObjects;
};
