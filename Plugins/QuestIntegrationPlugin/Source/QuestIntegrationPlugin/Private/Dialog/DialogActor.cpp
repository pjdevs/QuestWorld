// Copyright pjdevs. All Rights Reserved.


#include "Dialog/DialogActor.h"

#include "DialogComponent.h"


ADialogActor::ADialogActor()
	: DialogAsset(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADialogActor::DoInteraction_Implementation(AActor* InteractionInstigator)
{
	if (!DialogAsset)
	{
		return;
	}

	if (const APawn* InteractorPawn = Cast<APawn>(InteractionInstigator))
	{
		if (const APlayerController* Controller = Cast<APlayerController>(InteractorPawn->GetController()))
		{
			auto* DialogComponent = Cast<UDialogComponent>(
				Controller->GetComponentByClass(UDialogComponent::StaticClass())
			);

			if (DialogComponent)
			{
				DialogComponent->StartDialog(DialogAsset);
			}
		}
	}
}
