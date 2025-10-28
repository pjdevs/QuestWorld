// Copyright pjdevs. All Rights Reserved.


#include "InteractiveSaveSubsystem.h"
#include "EngineUtils.h"
#include "IPSavableInteractive.h"


class IIPSavableInteractive;

void UInteractiveSaveSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (!InWorld.GetAuthGameMode())
	{
		return;
	}

	for (TActorIterator<AActor> Actor(GetWorld()); Actor; ++Actor)
	{
		const IIPSavableInteractive* Savable = Cast<IIPSavableInteractive>(*Actor);

		if (Savable && Savable->IsSavable())
		{
			Actor->OnDestroyed.AddDynamic(this, &UInteractiveSaveSubsystem::OnSavableActorDestroyed);
			SavableActorArray.Add(*Actor);
		}
	}
}

void UInteractiveSaveSubsystem::LoadSaveData(const FInteractiveWorldSaveData& WorldSaveData)
{
	CurrentWorldSaveData = WorldSaveData;
	
	for (AActor* Actor : SavableActorArray)
	{
		if (IIPSavableInteractive* Savable = Cast<IIPSavableInteractive>(Actor))
		{
			if (const FInteractiveSaveData* InteractiveSave = WorldSaveData.SavedInteractives.Find(Savable->GetUniqueId()))
			{
				if (InteractiveSave->State.State == EIPInteractiveState::Destroyed)
				{
					Actor->Destroy();
				}
				else
				{
					Savable->LoadFromSave(*InteractiveSave);
				}
			}
		}
	}
}

const FInteractiveWorldSaveData& UInteractiveSaveSubsystem::GetSaveData()
{
	for (AActor* Actor : SavableActorArray)
	{
		if (IIPSavableInteractive* Savable = Cast<IIPSavableInteractive>(Actor))
		{
			FInteractiveSaveData& InteractiveSaveData = CurrentWorldSaveData.SavedInteractives.FindOrAdd(Savable->GetUniqueId());
			InteractiveSaveData = Savable->WriteToSave();
		}
	}
	
	return CurrentWorldSaveData;
}

void UInteractiveSaveSubsystem::OnSavableActorDestroyed(AActor* DestroyedActor)
{
	DestroyedActor->OnDestroyed.RemoveDynamic(this, &UInteractiveSaveSubsystem::OnSavableActorDestroyed);

	if (const IIPSavableInteractive* Savable = Cast<IIPSavableInteractive>(DestroyedActor))
	{
		FInteractiveSaveData& InteractiveSaveData = CurrentWorldSaveData.SavedInteractives.FindOrAdd(Savable->GetUniqueId());
		InteractiveSaveData.State.State = EIPInteractiveState::Destroyed;
	}
}
