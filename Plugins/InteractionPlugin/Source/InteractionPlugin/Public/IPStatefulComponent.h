// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ISpudObject.h"
#include "IPState.h"
#include "Components/ActorComponent.h"
#include "IPStatefulComponent.generated.h"


UCLASS(ClassGroup=(Interaction), meta=(BlueprintSpawnableComponent))
class INTERACTIONPLUGIN_API UIPStatefulComponent : public UActorComponent, public ISpudObject, public ISpudObjectCallback
{
	GENERATED_BODY()

public:
	UIPStatefulComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public: // Spud
	virtual bool ShouldSkip_Implementation() const override;
	virtual void SpudPostRestore_Implementation(const USpudState* SpudState) override;

public:
	UFUNCTION(BlueprintPure, Category = Interaction)
	EIPState GetState() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = Interaction)
	void SetState(EIPState NewState);

private:
	void OnStateChangedServer(EIPState OldState, EIPState NewState);
	
	UFUNCTION()
	void OnRep_State(const EIPState& OldState);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interaction)
	bool bIsPersistent;
	
protected:
	UPROPERTY(SaveGame, ReplicatedUsing=OnRep_State)
	EIPState State;
};
