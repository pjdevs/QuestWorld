// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/Actor/FlowNode_ComponentObserver.h"
#include "FlowNode_OnActorInteracted.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "On Actor Interacted"))
class QUESTINTEGRATIONPLUGIN_API UFlowNode_OnActorInteracted : public UFlowNode_ComponentObserver
{
	GENERATED_BODY()

public:
	UFlowNode_OnActorInteracted(const FObjectInitializer& ObjectInitializer);
	
protected:
	UFUNCTION()
	void OnActorInteracted(AActor* InteractionInstigator);
	
protected:
	virtual void ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;
	virtual void ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;
};
