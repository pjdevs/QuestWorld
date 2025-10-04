// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DialogWidget.h"
#include "Components/ActorComponent.h"
#include "DialogComponent.generated.h"


class UDialogNode;
class UDialogWidget;
class UDialogGraphAsset;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DIALOGPLUGIN_API UDialogComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = Dialog)
	void StartDialog(AActor* DialogActor, UDialogGraphAsset* DialogAsset);

private:
	void ExecuteCurrentDialogNode();
	void TryGoToChildNode(int NodeIndex);
	void EndDialog();
	
	void OnLineDisplayed();
	void OnChoicesDisplayed(int ChoiceIndex);

private:
	UPROPERTY(EditDefaultsOnly, Category = Dialog)
	TSubclassOf<UDialogWidget> DialogWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UDialogNode> CurrentNode;
	
	UPROPERTY()
	TObjectPtr<UDialogWidget> DisplayedDialogWidget;

	UPROPERTY()
	TObjectPtr<AController> OwnerController;

	UPROPERTY()
	TObjectPtr<AActor> CurrentDialogActor;

	TArray<int> AvailableChoiceIndexes;
};
