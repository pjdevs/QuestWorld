// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QIPHUDLayout.generated.h"

class UQIPMissionWidgetController;
class UQIPMissionPanel;
class UQIPAttributePanel;
class UQIPAttributeWidgetController;

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class QUESTINTEGRATIONPLUGIN_API UQIPHUDLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;
	
public:
	void SetGameState(AGameStateBase* InGameState);
	void SetPlayerState(APlayerState* InPlayerState);

private:
	UPROPERTY()
	TObjectPtr<UQIPAttributeWidgetController> AttributeController;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UQIPAttributePanel> AttributePanel;

	UPROPERTY()
	TObjectPtr<UQIPMissionWidgetController> MissionController;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UQIPMissionPanel> MissionPanel;
	
private:
	UPROPERTY()
	TObjectPtr<AGameStateBase> GameState;
	
	UPROPERTY()
	TObjectPtr<APlayerState> PlayerState;
};
