// Copyright pjdevs. All Rights Reserved.

#pragma once

UENUM()
enum class EQuestCompletionState : uint8
{
	Started,
	Succeeded,
	Failed,
	Abandoned
};