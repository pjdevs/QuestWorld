// Copyright pjdevs. All Rights Reserved.


#include "QuestPhaseReferenceCustomization.h"

#include "Assets/QuestDataAsset.h"
#include "Assets/QuestPhaseReference.h"
#include "DetailWidgetRow.h"
#include "PropertyHandleUtils.h"


TSharedRef<IPropertyTypeCustomization> FQuestPhaseReferenceCustomization::MakeInstance()
{
    return MakeShareable(new FQuestPhaseReferenceCustomization());
}

void FQuestPhaseReferenceCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
    QuestRefHandle = StructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FQuestPhaseReference, QuestRef));
    PhaseHandle = StructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FQuestPhaseReference, Phase));

    TSoftObjectPtr<UQuestDataAsset> QuestRef = nullptr;
    
    UObject* Owner = nullptr;
    TArray<UObject*> OuterObjects;
    StructHandle->GetOuterObjects(OuterObjects);
    if (OuterObjects.Num() > 0)
    {
        Owner = OuterObjects[0];
    }

    FString GetQuestRefFuncName = StructHandle->GetProperty()->GetMetaData("QuestReference");
    if (!GetQuestRefFuncName.IsEmpty() && Owner)
    {
        if (UFunction* GetQuestRefFunc = Owner->FindFunction(FName(*GetQuestRefFuncName)))
        {
            struct FReturn { TSoftObjectPtr<UQuestDataAsset> QuestRef; } ReturnValue;
            Owner->ProcessEvent(GetQuestRefFunc, &ReturnValue);
            QuestRef = ReturnValue.QuestRef;
        }
    }

    FQuestPhaseReference* PhaseRef = GetHandleValue<FQuestPhaseReference>(StructHandle);

    if (!PhaseRef->IsValid() || PhaseRef->QuestRef != QuestRef)
    {
        StructHandle->NotifyPreChange();
        SetHandleValue(QuestRefHandle, QuestRef);
        PhaseRef->FixupReference();
        StructHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
        StructHandle->NotifyFinishedChangingProperties();
    }

    Phases.Empty();

    if (const UQuestDataAsset* QuestAsset = QuestRef.LoadSynchronous())
    {
        for (const FName& Phase : QuestAsset->Phases)
        {
            Phases.Add(Phase);   
        }
    }
    
    HeaderRow
        .NameContent()[StructHandle->CreatePropertyNameWidget()]
        .ValueContent()
        [
            SAssignNew(ComboBox, SComboBox<FName>)
                .OptionsSource(&Phases)
                .Content()
                [
                    SNew(STextBlock)
                        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
                        .Text(this, &FQuestPhaseReferenceCustomization::GetContentText)
                ]
                .OnGenerateWidget_Raw(this, &FQuestPhaseReferenceCustomization::GenerateWidget)
                .OnSelectionChanged_Raw(this, &FQuestPhaseReferenceCustomization::OnSelectionChanged)
                .InitiallySelectedItem(PhaseRef->Phase)
        ];
}

void FQuestPhaseReferenceCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructHandle,
    IDetailChildrenBuilder& ChildBuilder,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
}

FText FQuestPhaseReferenceCustomization::GetContentText() const
{
    return FText::FromName(ComboBox->GetSelectedItem());
}

TSharedRef<SWidget> FQuestPhaseReferenceCustomization::GenerateWidget(FName Name) const
{
    return SNew(STextBlock)
        .Text(FText::FromName(Name))
        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"));
}

void FQuestPhaseReferenceCustomization::OnSelectionChanged(FName SelectedObjectiveId, ESelectInfo::Type SelectionType) const
{
    PhaseHandle->NotifyPreChange();
    SetHandleValue(PhaseHandle, SelectedObjectiveId);
    PhaseHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    PhaseHandle->NotifyFinishedChangingProperties();
}
