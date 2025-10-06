// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogNode.h"
#include "SingleDialogNode.generated.h"

/**
 * 
 */
UCLASS()
class DIALOGPLUGIN_API USingleDialogNode : public UDialogNode
{
	GENERATED_BODY()

public:
	const FText& GetLine() const { return DialogLine; }
	void SetLine(const FText& Line) { DialogLine = Line; }
	
private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog, meta = (AllowPrivateAccess = true))
	FText DialogLine;
};
