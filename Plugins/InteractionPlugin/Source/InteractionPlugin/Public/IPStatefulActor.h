// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPStateHandler.h"
#include "ISpudObject.h"
#include "GameFramework/Actor.h"
#include "IPStatefulActor.generated.h"

class UIPStatefulComponent;

UCLASS()
class INTERACTIONPLUGIN_API AIPStatefulActor : public AActor, public IIPStateHandler, public ISpudObject
{
	GENERATED_BODY()

public:
	AIPStatefulActor();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, SaveGame, Category = Interaction)
	TObjectPtr<UIPStatefulComponent> StatefulComponent;
};
