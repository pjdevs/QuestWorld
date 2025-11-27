// Copyright pjdevs. All Rights Reserved.

#pragma once

UENUM(BlueprintType)
enum class EQuestCompletionState : uint8
{
	Started,
	Succeeded,
	Failed,
	Abandoned
};