// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "QuestFlowAssetFactory.generated.h"

/**
 * 
 */
UCLASS(Hidecategories = Object)
class QUESTSYSTEMEDITOR_API UQuestFlowAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UQuestFlowAssetFactory(const FObjectInitializer& ObjectInitializer);
	
	virtual UObject* FactoryCreateNew(
		UClass* InClass,
		UObject* InParent,
		FName InName,
		EObjectFlags Flags,
		UObject* Context,
		FFeedbackContext* Warn
	) override;
};
