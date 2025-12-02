// Copyright pjdevs. All Rights Reserved.


#include "Assets/QuestDataAssetValidator.h"

#include "Assets/QuestDataAsset.h"
#include "Flow/QuestFlowAsset.h"
#include "Misc/DataValidation.h"


bool UQuestDataAssetValidator::CanValidateAsset_Implementation(
	const FAssetData& InAssetData,
	UObject* InObject,
	FDataValidationContext& InContext
) const
{
	return InObject && InObject->IsA<UQuestDataAsset>();
}

EDataValidationResult UQuestDataAssetValidator::ValidateLoadedAsset_Implementation(
	const FAssetData& InAssetData,
	UObject* InAsset,
	FDataValidationContext& Context
)
{
	const UQuestDataAsset* QuestAsset = Cast<UQuestDataAsset>(InAsset);

	if (!QuestAsset)
	{
		return EDataValidationResult::NotValidated;
	}
	
	if (QuestAsset->Objectives.Num() == 0)
	{
		Context.AddError(FText::FromString("Quest must contain at least one objective."));
		return EDataValidationResult::Invalid;
	}

	for (const auto& Obj : QuestAsset->Objectives)
	{
		if (!Obj->Phases.Phases.IsEmpty() && !QuestAsset->Phases.Includes(Obj->Phases.Phases))
		{
			Context.AddError(
				FText::FromString(
					FString::Printf(
						TEXT("Objective %s references phases that are not defined in quest asset."),
						*Obj->ObjectiveId.ToString()
					)
				)
			);
			return EDataValidationResult::Invalid;
		}
	}

	const bool bIsFlowNull = QuestAsset->QuestFlowAsset.IsNull();
	const UQuestFlowAsset* QuestFlowAsset = QuestAsset->QuestFlowAsset.LoadSynchronous();

	if (!bIsFlowNull && (!QuestFlowAsset || QuestFlowAsset->LinkedQuestRef != QuestAsset))
	{
		Context.AddError(
			FText::FromString(
				FString::Printf(
					 TEXT("Cannot reference quest flow %s because it is invalid or references an other quest."),
					*QuestAsset->QuestFlowAsset.ToSoftObjectPath().GetAssetFName().ToString()
				)
			)
		);
		return EDataValidationResult::Invalid;
	}

	// OK
	return EDataValidationResult::Valid;
}
