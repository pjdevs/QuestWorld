// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UQuestDataAsset;

/**
 * Customization to allow only parent quest phase selection.
 */
class FQuestPhaseReferencesCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

private:
	virtual void CustomizeHeader(
		TSharedRef<IPropertyHandle> StructHandle,
		FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils
	) override;

	virtual void CustomizeChildren(
	TSharedRef<IPropertyHandle> StructHandle,
		IDetailChildrenBuilder& ChildBuilder,
		IPropertyTypeCustomizationUtils& CustomizationUtils
	) override;


private:
	TSharedRef<SWidget> BuildMenu();
	void TogglePhase(FName Phase) const;
	FText GetSummary() const;

	TSet<FName>* GetCurrentPhases() const;
	UQuestDataAsset* GetOwnerQuestAsset() const;
	
private:
	TSharedPtr<IPropertyHandle> ThisStructHandle;
	TSharedPtr<IPropertyHandle> PhasesHandle;
};