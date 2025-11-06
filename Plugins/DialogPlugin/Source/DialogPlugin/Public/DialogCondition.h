// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DialogCondition.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class DIALOGPLUGIN_API UDialogCondition : public UObject
{
	GENERATED_BODY()

public:
	virtual UWorld* GetWorld() const override;
	
public:
	bool IsSatisfied(UWorld* InWorld, AController* DialogController);

protected:
	UFUNCTION(BlueprintNativeEvent, Category = Dialog)
	bool IsSatisfied(AController* DialogController);
	virtual bool IsSatisfied_Implementation(AController* DialogController);

private:
	UPROPERTY(Transient)
	UWorld* World = nullptr;
};
