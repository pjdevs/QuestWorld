// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPStateSavable.h"
#include "IPState.h"
#include "GameFramework/Actor.h"
#include "IPStatefulActor.generated.h"

// TODO See if we refactor AIPInteractiveActor with this class

UCLASS()
class INTERACTIONPLUGIN_API AIPStatefulActor : public AActor, public IIPStateSavable
{
	GENERATED_BODY()

public:
	AIPStatefulActor();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public: // IIPStateSavable interface
	virtual FName GetUniqueId() const override;
	virtual bool IsSavable() const override;
	virtual void LoadFromSave(const FIPStateSaveData& SaveData) override;
	virtual FIPStateSaveData WriteToSave() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetState(EIPState NewState);
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnStateChanged(EIPState NewState);
	void OnStateChanged_Implementation(EIPState NewState);

private:
	UFUNCTION()
	void OnRep_State();
	
private:
	UPROPERTY(EditAnywhere, Category = "Save", meta = (AllowPrivateAccess = true))
	FName UniqueId;

	UPROPERTY(EditAnywhere, Category = "Save", meta = (AllowPrivateAccess = true))
	bool bIsSavable;

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_State)
	EIPState State;
};
