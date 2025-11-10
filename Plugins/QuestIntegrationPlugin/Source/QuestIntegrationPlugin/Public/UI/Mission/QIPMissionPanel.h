// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QIPMissionPanel.generated.h"

struct FQuestDescription;
class UQIPMissionWidgetController;
/**
 * 
 */
UCLASS()
class QUESTINTEGRATIONPLUGIN_API UQIPMissionPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeDestruct() override;

public:
	void SetWidgetController(UQIPMissionWidgetController* InWidgetController);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Mission)
	void UpdateMission(const FQuestDescription& MissionDescription);
	
	UFUNCTION(BlueprintImplementableEvent, Category = Mission)
	void HideMission();

private:
	UPROPERTY()
	TObjectPtr<UQIPMissionWidgetController> WidgetController;

	FDelegateHandle MissionUpdatedDelegateHandle;
	FDelegateHandle MissionHiddenDelegateHandle;
};
