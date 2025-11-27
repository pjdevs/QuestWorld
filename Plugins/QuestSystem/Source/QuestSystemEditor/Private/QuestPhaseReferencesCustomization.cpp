// Copyright pjdevs. All Rights Reserved.


#include "QuestPhaseReferencesCustomization.h"

#include "DetailWidgetRow.h"
#include "Assets/QuestDataAsset.h"
#include "Assets/QuestPhaseReferences.h"


TSharedRef<IPropertyTypeCustomization> FQuestPhaseReferencesCustomization::MakeInstance()
{
    return MakeShareable(new FQuestPhaseReferencesCustomization());
}

void FQuestPhaseReferencesCustomization::CustomizeHeader(
    TSharedRef<IPropertyHandle> StructHandle,
    FDetailWidgetRow& HeaderRow,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
    ThisStructHandle = StructHandle;
    PhasesHandle = ThisStructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FQuestPhaseReferences, Phases));

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
                .OnGetMenuContent(this, &FQuestPhaseReferencesCustomization::BuildMenu)
                .ButtonContent()
                [
                    SNew(STextBlock)
                        .Text(this, &FQuestPhaseReferencesCustomization::GetSummary)
                        .Font(FAppStyle::GetFontStyle("PropertyWindow.NormalFont"))
                ]
        ];
}

void FQuestPhaseReferencesCustomization::CustomizeChildren(
    TSharedRef<IPropertyHandle> StructHandle,
    IDetailChildrenBuilder& ChildBuilder,
    IPropertyTypeCustomizationUtils& CustomizationUtils
)
{
}

TSharedRef<SWidget> FQuestPhaseReferencesCustomization::BuildMenu()
{
    FMenuBuilder Menu(true, nullptr);

    UQuestDataAsset* OwnerQuestAsset = GetOwnerQuestAsset();
    
    if (!OwnerQuestAsset)
    {
        return Menu.MakeWidget();
    }

    for (const FName& Phase : OwnerQuestAsset->Phases)
    {
        const TSet<FName>* CurrentPhases = GetCurrentPhases();
        TAttribute<bool> bIsPhaseSelected = TAttribute<bool>::Create(
            [CurrentPhases, Phase] { return CurrentPhases->Contains(Phase); }
        );
        
        FUIAction Action(
            FExecuteAction::CreateSP(this, &FQuestPhaseReferencesCustomization::TogglePhase, Phase),
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

void FQuestPhaseReferencesCustomization::TogglePhase(FName Phase) const
{
    TSet<FName>* CurrentPhases = GetCurrentPhases();

    PhasesHandle->NotifyPreChange();
    
    if (CurrentPhases->Contains(Phase))
    {
        CurrentPhases->Remove(Phase);
    }
    else
    {
        CurrentPhases->Add(Phase);
    }

    PhasesHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
    PhasesHandle->NotifyFinishedChangingProperties();
}

FText FQuestPhaseReferencesCustomization::GetSummary() const
{
    TSet<FName>* CurrentPhases = GetCurrentPhases();

    if (CurrentPhases->IsEmpty())
    {
        return FText::FromString("None");
    }

    FString SummaryString;

    for (const FName& Phase : *CurrentPhases)
    {
        SummaryString += Phase.ToString();
        SummaryString += "\n";
    }
    
    return FText::FromString(SummaryString);
}

TSet<FName>* FQuestPhaseReferencesCustomization::GetCurrentPhases() const
{
    void* CurrentPtr;
    PhasesHandle->GetValueData(CurrentPtr);
    return static_cast<TSet<FName>*>(CurrentPtr);
}

UQuestDataAsset* FQuestPhaseReferencesCustomization::GetOwnerQuestAsset() const
{
    TArray<UObject*> Outers;
    ThisStructHandle->GetOuterObjects(Outers);

    if (Outers.Num() > 0)
    {
        if (const IQuestOwnedObjectInterface* QuestOwnedObject = Cast<IQuestOwnedObjectInterface>(Outers[0]))
        {
            return QuestOwnedObject->GetOwningQuest();
        }
    }

    return nullptr;
}
