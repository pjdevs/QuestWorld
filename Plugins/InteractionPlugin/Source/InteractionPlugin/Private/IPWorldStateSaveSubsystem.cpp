// Copyright pjdevs. All Rights Reserved.


#include "IPWorldStateSaveSubsystem.h"
#include "EngineUtils.h"
#include "IPStateSavable.h"


void UIPWorldStateSaveSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	if (!InWorld.GetAuthGameMode())
	{
		return;
	}

	for (TActorIterator<AActor> Actor(GetWorld()); Actor; ++Actor)
	{
		const IIPStateSavable* Savable = Cast<IIPStateSavable>(*Actor);

		if (Savable && Savable->IsSavable())
		{
			Actor->OnDestroyed.AddDynamic(this, &UIPWorldStateSaveSubsystem::OnSavableActorDestroyed);
			SavableActorArray.Add(*Actor);
		}
	}
	//
	// OnActorSpawnedDelegateHandle = InWorld.AddOnActorSpawnedHandler(FOnActorSpawned::FDelegate::CreateUObject(
	// 	this,
	// 	&UIPWorldStateSaveSubsystem::OnActorSpawned
	// ));
}

void UIPWorldStateSaveSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UIPWorldStateSaveSubsystem::LoadSaveData(const FIPWorldStateSaveData& WorldSaveData)
{
	CurrentWorldSaveData = WorldSaveData;

	TArray<AActor*> ActorsToDestroy;
	
	for (AActor* Actor : SavableActorArray)
	{
		if (IIPStateSavable* Savable = Cast<IIPStateSavable>(Actor))
		{
			if (const FIPStateSaveData* InteractiveSave = WorldSaveData.SavedActors.Find(Savable->GetUniqueId()))
			{
				if (InteractiveSave->bWasDestroyed)
				{
					ActorsToDestroy.Add(Actor);
				}
				else
				{
					Savable->LoadFromSave(*InteractiveSave);
				}
			}
		}
	}

	for (AActor* Actor : ActorsToDestroy)
	{
		Actor->Destroy();
	}
}

const FIPWorldStateSaveData& UIPWorldStateSaveSubsystem::GetSaveData()
{
	for (AActor* Actor : SavableActorArray)
	{
		if (IIPStateSavable* Savable = Cast<IIPStateSavable>(Actor))
		{
			FIPStateSaveData& InteractiveSaveData = CurrentWorldSaveData.SavedActors.FindOrAdd(Savable->GetUniqueId());
			InteractiveSaveData = Savable->WriteToSave();
		}
	}
	
	return CurrentWorldSaveData;
}

// void UIPWorldStateSaveSubsystem::OnActorSpawned(AActor* Actor)
// {
// 	const IIPStateSavable* Savable = Cast<IIPStateSavable>(Actor);
// 	
// 	if (Savable && Savable->IsSavable())
// 	{
// 		SavableActorArray.Add(Actor);
// 	}
// }

void UIPWorldStateSaveSubsystem::OnSavableActorDestroyed(AActor* DestroyedActor)
{
	DestroyedActor->OnDestroyed.RemoveDynamic(this, &UIPWorldStateSaveSubsystem::OnSavableActorDestroyed);

	if (const IIPStateSavable* Savable = Cast<IIPStateSavable>(DestroyedActor))
	{
		FIPStateSaveData& InteractiveSaveData = CurrentWorldSaveData.SavedActors.FindOrAdd(Savable->GetUniqueId());
		InteractiveSaveData.bWasDestroyed = true;
	}

	SavableActorArray.Remove(DestroyedActor);
}
