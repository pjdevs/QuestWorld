// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CharacterMovementByTagComponent.generated.h"


class ACharacter;
class UAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEPLAYABILITYSYSTEMPLUGIN_API UCharacterMovementByTagComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterMovementByTagComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:	
	void OnMovementBlockedTagCountChanged(FGameplayTag GameplayTag, int TagCount);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Movement, meta = (AllowPrivateAccess = true))
	FGameplayTag MovementBlockedTag;

private:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	UAbilitySystemComponent* OwnerAsc;

	FDelegateHandle TagCountChangedDelegateHandle;
	EMovementMode LastMovementMode;
	bool bLastUseControllerRotationYaw;
};
