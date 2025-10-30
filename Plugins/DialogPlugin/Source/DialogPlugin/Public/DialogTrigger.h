// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogTrigger.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class DIALOGPLUGIN_API UDialogTrigger : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;
	
public:
	void Execute(UWorld* InWorld, AController* DialogController);

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Dialog)
	void OnExecute(AController* DialogController);
	virtual void OnExecute_Implementation(AController* DialogController);

private:
	UPROPERTY(Transient)
	UWorld* World = nullptr;
};
