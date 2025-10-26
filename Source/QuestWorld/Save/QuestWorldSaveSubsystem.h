// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "QuestWorldSaveSubsystem.generated.h"

class UQuestWorldSaveGame;
/**
 * 
 */
UCLASS()
class QUESTWORLD_API UQuestWorldSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void LoadSaveGame();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void LoadGame();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Save)
	void SaveGame();

private:
	UPROPERTY()
	UQuestWorldSaveGame* SaveGameObject;

	FString SaveGameSlotName = "QuestWorldSave";
};
