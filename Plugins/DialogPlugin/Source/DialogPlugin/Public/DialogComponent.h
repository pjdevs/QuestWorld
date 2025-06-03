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
	UDialogComponent();
	
	void StartDialog(UDialogDataAsset* DialogAsset);

private:
	void ExecuteCurrentDialogNode();
	void EndDialog();

	UFUNCTION()
	void OnLineDisplayed();
	UFUNCTION()
	void OnChoicesDisplayed(int ChoiceIndex);

private:
	UPROPERTY()
	TObjectPtr<UDialogNode> CurrentNode;
	
	UPROPERTY()
	TObjectPtr<UDialogWidget> DisplayedDialogWidget;

	UPROPERTY()
	FDisplayLineFinishedDelegate DisplayLineFinishedDelegate;

	UPROPERTY()
	FDisplayChoicesFinishedDelegate DisplayChoicesFinishedDelegate;
};
