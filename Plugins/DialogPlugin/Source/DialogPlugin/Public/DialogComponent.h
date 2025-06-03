// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "Components/ActorComponent.h"
#include "DialogComponent.generated.h"


class UDialogNode;
class UDialogWidget;
class UDialogDataAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGPLUGIN_API UDialogComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	TSubclassOf<UDialogWidget> DialogWidgetClass;
	
public:
	UFUNCTION(BlueprintCallable, Category = Dialog)
	void StartDialog(UDialogDataAsset* DialogAsset);

private:
	void ExecuteCurrentDialogNode();
	void EndDialog();
	
	void OnLineDisplayed();
	void OnChoicesDisplayed(int ChoiceIndex);

private:
	UPROPERTY()
	TObjectPtr<UDialogNode> CurrentNode;
	
	UPROPERTY()
	TObjectPtr<UDialogWidget> DisplayedDialogWidget;
};
