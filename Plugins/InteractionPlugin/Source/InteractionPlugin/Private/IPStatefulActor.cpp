// Copyright pjdevs. All Rights Reserved.


#include "IPStatefulActor.h"

#include "IPStateSaveData.h"
#include "Net/UnrealNetwork.h"


AIPStatefulActor::AIPStatefulActor()
	: UniqueId(NAME_None), bIsSavable(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	bReplicates = true;
}

void AIPStatefulActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIPStatefulActor, State);
}

FName AIPStatefulActor::GetUniqueId() const
{
	return UniqueId;
}

bool AIPStatefulActor::IsSavable() const
{
	return bIsSavable;
}

void AIPStatefulActor::LoadFromSave(const FIPStateSaveData& SaveData)
{
	State = static_cast<EIPState>(SaveData.State);
	OnRep_State();
}

FIPStateSaveData AIPStatefulActor::WriteToSave()
{
	return FIPStateSaveData
	{
		.State = static_cast<uint8>(State),
		.Counter = 0 // unused for this class
	};
}

void AIPStatefulActor::SetState(EIPState NewState)
{
	if (!HasAuthority())
	{
		return;
	}

	State = NewState;
	OnRep_State();
}

void AIPStatefulActor::DoFeedback_Implementation(EIPState NewState)
{
	
}

void AIPStatefulActor::OnRep_State()
{
	DoFeedback(State);
}

