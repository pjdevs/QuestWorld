// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#if WITH_SPUD
#include "ISpudObject.h"
#endif
#include "IPState.h"
#include "Components/ActorComponent.h"
#include "IPStatefulComponent.generated.h"


UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class INTERACTIONPLUGIN_API UIPStatefulComponent : public UActorComponent
#if WITH_SPUD
	, public ISpudObject, public ISpudObjectCallback
#endif
{
	GENERATED_BODY()

public:
	UIPStatefulComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

#if WITH_SPUD
public:
	virtual bool ShouldSkip_Implementation() const override;
	virtual void SpudPostRestore_Implementation(const USpudState* SpudState) override;
#endif

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Interaction)
	void SetState(EIPState NewState);

private:
	UFUNCTION()
	void OnRep_State(const EIPState& OldState);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Interaction)
	bool bIsPersistent;
	
protected:
	UPROPERTY(SaveGame, BlueprintReadOnly, ReplicatedUsing=OnRep_State, Category = Interaction)
	EIPState State;
};
