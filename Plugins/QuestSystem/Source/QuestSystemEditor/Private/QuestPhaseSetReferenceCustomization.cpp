// Copyright pjdevs. All Rights Reserved.


#include "QuestPhaseSetReferenceCustomization.h"

#include "DetailWidgetRow.h"
#include "Assets/QuestDataAsset.h"
#include "Assets/QuestPhaseSetReference.h"
#include "PropertyHandleUtils.h"


TSharedRef<IPropertyTypeCustomization> FQuestPhaseSetReferenceCustomization::MakeInstance()
{
    return MakeShareable(new FQuestPhaseSetReferenceCustomization());
}

void FQuestPhaseSetReferenceCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
    ThisStructHandle = StructHandle;
    PhasesHandle = ThisStructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FQuestPhaseSetReference, Phases));

    HeaderRow
        .NameContent()
        [
            SNew(STextBlock)
                .Text(FText::FromString("Phases"))
                .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
        ]
        .ValueContent()
        [
            SNew(SComboButton)
                .OnGetMenuContent(this, &FQuestPhaseSetReferenceCustomization::BuildMenu)
                .ButtonContent()
                [
                    SNew(STextBlock)
                        .Text(this, &FQuestPhaseSetReferenceCustomization::GetSummary)
                        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
                ]
        ];
}

void FQuestPhaseSetReferenceCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructHandle,
    IDetailChildrenBuilder& ChildBuilder,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
}

TSharedRef<SWidget> FQuestPhaseSetReferenceCustomization::BuildMenu()
{
    FMenuBuilder Menu(true, nullptr);

    const UQuestDataAsset* OwnerQuestAsset = GetOwnerQuestAsset();
    
    if (!OwnerQuestAsset)
    {
        return Menu.MakeWidget();
    }

    for (const FName& Phase : OwnerQuestAsset->Phases)
    {
        const FQuestPhaseSetReference* CurrentPhases = GetHandleValue<FQuestPhaseSetReference>(ThisStructHandle);
        TAttribute<bool> bIsPhaseSelected = TAttribute<bool>::Create(
            [CurrentPhases, Phase] { return CurrentPhases->Phases.Contains(Phase); }
        );
        
        FUIAction Action(
            FExecuteAction::CreateSP(this, &FQuestPhaseSetReferenceCustomization::TogglePhase, Phase),
            FCanExecuteAction(),
            FIsActionChecked::CreateLambda([bIsPhaseSelected] { return bIsPhaseSelected.Get(); })
        );
        
        Menu.AddMenuEntry(
            FText::FromName(Phase),
            FText::GetEmpty(),
            FSlateIcon(),
            Action,
            NAME_None,
            EUserInterfaceActionType::ToggleButton
        );
    }

    return Menu.MakeWidget();
}

void FQuestPhaseSetReferenceCustomization::TogglePhase(FName Phase) const
{
    FQuestPhaseSetReference* CurrentPhases = GetHandleValue<FQuestPhaseSetReference>(ThisStructHandle);

    PhasesHandle->NotifyPreChange();
    
    if (CurrentPhases->Phases.Contains(Phase))
    {
        CurrentPhases->Phases.Remove(Phase);
    }
    else
    {
        CurrentPhases->Phases.Add(Phase);
    }

    PhasesHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    PhasesHandle->NotifyFinishedChangingProperties();
}

FText FQuestPhaseSetReferenceCustomization::GetSummary() const
{
    const FQuestPhaseSetReference* CurrentPhases = GetHandleValue<FQuestPhaseSetReference>(ThisStructHandle);

    if (CurrentPhases->Phases.IsEmpty())
    {
        return FText::FromString("None");
    }
    
    return FText::FromString(CurrentPhases->ToString());
}

const UQuestDataAsset* FQuestPhaseSetReferenceCustomization::GetOwnerQuestAsset() const
{
    TArray<UObject*> Outers;
    ThisStructHandle->GetOuterObjects(Outers);

    if (Outers.Num() > 0)
    {
        if (const UQuestDataAsset* OwningQuest = Cast<UQuestDataAsset>(Outers[0]->GetOuter()))
        {
            return OwningQuest;
        }
    }

    return nullptr;
}
