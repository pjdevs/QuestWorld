// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GaspCharacterBase.generated.h"

class UGameplayAbility;
class UGaspAbilitySystemComponent;

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
	
protected:
	UPROPERTY()
	TObjectPtr<UGaspAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = Ability)
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;
};
