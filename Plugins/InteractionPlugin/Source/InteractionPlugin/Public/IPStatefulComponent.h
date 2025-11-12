// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPState.h"
#include "Components/ActorComponent.h"
#include "IPStatefulComponent.generated.h"


UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class INTERACTIONPLUGIN_API UIPStatefulComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UIPStatefulComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Interaction)
	void SetState(EIPState NewState);

private:
	UFUNCTION()
	void OnRep_State(const EIPState& OldState);

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_State, Category = Interaction)
	EIPState State;
};
