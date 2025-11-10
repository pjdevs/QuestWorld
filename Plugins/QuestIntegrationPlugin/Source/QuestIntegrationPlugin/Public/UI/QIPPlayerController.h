// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QIPPlayerController.generated.h"

class UQIPHUDLayout;

/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API AQIPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnRep_PlayerState() override;

private:
	void OnGameStateSet(AGameStateBase* SetGameState); 
	
private:
	UPROPERTY(EditDefaultsOnly, Category = HUD, meta = (AllowPrivateAccess = true))
	TSubclassOf<UQIPHUDLayout> HUDWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UQIPHUDLayout> HUDLayout;

	FDelegateHandle GameStateSetHandle;
};
