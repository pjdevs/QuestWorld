// Copyright pjdevs. All Rights Reserved.


#include "Dialog/DialogActor.h"

#include "DialogComponent.h"


ADialogActor::ADialogActor()
	: DialogAsset(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADialogActor::DoInteract(AActor* InteractionInstigator)
{
	if (!DialogAsset)
		return;

	if (const APawn* InteractorPawn = Cast<APawn>(InteractionInstigator))
	{
		if (const APlayerController* Controller = Cast<APlayerController>(InteractorPawn->GetController()))
		{
			UDialogComponent* DialogComponent = Cast<UDialogComponent>(
				Controller->GetComponentByClass(UDialogComponent::StaticClass())
			);

			if (DialogComponent)
			{
				DialogComponent->StartDialog(DialogAsset);
			}
		}
	}
	
}
