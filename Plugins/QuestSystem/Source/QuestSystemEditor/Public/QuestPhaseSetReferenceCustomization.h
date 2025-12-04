// Copyright pjdevs. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Assets/QuestPhaseSetReference.h"

class UQuestDataAsset;

/**
 * Customization to allow only parent quest phase selection.
 */
class FQuestPhaseSetReferenceCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

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

	const UQuestDataAsset* GetOwnerQuestAsset() const;
	
private:
	TSharedPtr<IPropertyHandle> ThisStructHandle;
	TSharedPtr<IPropertyHandle> PhasesHandle;
};