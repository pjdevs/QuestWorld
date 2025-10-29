// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GaspCharacterBase.generated.h"

class UGameplayEffect;
class UGaspAttributeSet;
class UGameplayAbility;
class UGaspAbilitySystemComponent;

DECLARE_DELEGATE(FAscInitializedDelegate);

UCLASS()
class GAMEPLAYABILITYSYSTEMPLUGIN_API AGaspCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AGaspCharacterBase();

public: // IAbilitySystemInterface interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	void GiveDefaultAbilities();
	void InitDefaultAttributes();
	void ApplyDefaultPermanentEffects();

public:
	FAscInitializedDelegate OnAscInitialized;
	
protected:
	UPROPERTY()
	TObjectPtr<UGaspAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UGaspAttributeSet> AttributeSet;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Ability, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, Category = Ability, meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	UPROPERTY(EditDefaultsOnly, Category = Ability, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<UGameplayEffect>> DefaultPermanentEffects;
};
