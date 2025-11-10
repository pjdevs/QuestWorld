// Copyright pjdevs. All Rights Reserved.


#include "UI/Mission/QIPMissionPanel.h"
#include "UI/Mission/QIPMissionWidgetController.h"

void UQIPMissionPanel::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(WidgetController))
	{
		WidgetController->OnMissionUpdated.Remove(MissionHiddenDelegateHandle);
		WidgetController->OnNoMission.Remove(MissionHiddenDelegateHandle);
	}
}

void UQIPMissionPanel::SetWidgetController(UQIPMissionWidgetController* InWidgetController)
{
	if (!IsValid(InWidgetController))
	{
		return;
	}
	
	WidgetController = InWidgetController;

	MissionUpdatedDelegateHandle = WidgetController->OnMissionUpdated.AddUObject(this, &UQIPMissionPanel::UpdateMission);
	MissionHiddenDelegateHandle = WidgetController->OnNoMission.AddUObject(this, &UQIPMissionPanel::HideMission);

	WidgetController->TriggerInitialEvents();
}
