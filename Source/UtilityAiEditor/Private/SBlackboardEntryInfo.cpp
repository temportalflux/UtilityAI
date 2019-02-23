#include "SBlackboardEntryInfo.h"

#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SComboButton.h"

#include "SSelectorBlackboardEntry.h"
#include "SSelectorBlackboardVarType.h"
#include "UtilityAiEditor.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SBlackboardEntryInfo"

void SBlackboardEntryInfo::Construct(const FArguments & InArgs)
{
	this->mpBlackboard = InArgs._BlackboardAsset.Get();
	mIsCreatingEntry = false;

	ChildSlot
	[
		SNew(SVerticalBox)

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SSelectorBlackboardEntry)
				.BlackboardAsset(this->mpBlackboard)
				.OnClearSelection(FOnClearSelection::CreateRaw(
					this, &SBlackboardEntryInfo::OnClearEntrySelection))
				.OnSelection(FOnSelectedEntry::CreateRaw(
					this, &SBlackboardEntryInfo::OnMakeEntrySelection))
			]

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SAssignNew(mpWidgetEntryVars, SVerticalBox)
			]

	];
}

void SBlackboardEntryInfo::OnClearEntrySelection()
{
	if (mIsCreatingEntry) return;

	mIsCreatingEntry = true;
	this->mpWidgetEntryVars->AddSlot()
	[
		SNew(SSelectorBlackboardVarType)
		.OnSelected(FOnSelectedBlackboardVarType::CreateRaw(
			this, &SBlackboardEntryInfo::OnSelectKeyType
		))
	];
}

void SBlackboardEntryInfo::OnMakeEntrySelection(FBlackboardEntry entry)
{
	if (mIsCreatingEntry)
	{
		this->mpWidgetEntryVars->ClearChildren();
	}

	mIsCreatingEntry = false;
	
	UE_LOG(LogUtilityAiEditor, Log, TEXT("%s"), *entry.EntryName.ToString());
}

void SBlackboardEntryInfo::OnSelectKeyType(UClass* InClass)
{
	/*
	const FScopedTransaction Transaction(LOCTEXT("BlackboardEntryAddTransaction", "Add Blackboard Entry"));
	BlackboardData->SetFlags(RF_Transactional);
	BlackboardData->Modify();

	// create a name for this new key
	FString NewKeyName = InClass->GetDisplayNameText().ToString();
	NewKeyName = NewKeyName.Replace(TEXT(" "), TEXT(""));
	NewKeyName += TEXT("Key");

	int32 IndexSuffix = -1;
	auto DuplicateFunction = [&](const FBlackboardEntry& Key)
	{		
		if(Key.EntryName.ToString() == NewKeyName)
		{
			IndexSuffix = FMath::Max(0, IndexSuffix);
		}
		if(Key.EntryName.ToString().StartsWith(NewKeyName))
		{
			const FString ExistingSuffix = Key.EntryName.ToString().RightChop(NewKeyName.Len());
			if(ExistingSuffix.IsNumeric())
			{
				IndexSuffix = FMath::Max(FCString::Atoi(*ExistingSuffix) + 1, IndexSuffix);
			}
		}
	};

	// check for existing keys of the same name
	for(const auto& Key : BlackboardData->Keys) { DuplicateFunction(Key); };
	for(const auto& Key : BlackboardData->ParentKeys) { DuplicateFunction(Key); };

	if(IndexSuffix != -1)
	{
		NewKeyName += FString::Printf(TEXT("%d"), IndexSuffix);
	}

	FBlackboardEntry Entry;
	Entry.EntryName = FName(*NewKeyName);
	Entry.KeyType = NewObject<UBlackboardKeyType>(BlackboardData, InClass);

	BlackboardData->Keys.Add(Entry);

	GraphActionMenu->RefreshAllActions(true);
	OnBlackboardKeyChanged.ExecuteIfBound(BlackboardData, &BlackboardData->Keys.Last());

	GraphActionMenu->SelectItemByName(Entry.EntryName, ESelectInfo::OnMouseClick);

	// Mark newly created entry as 'new'
	TArray< TSharedPtr<FEdGraphSchemaAction> > SelectedActions;
	GraphActionMenu->GetSelectedActions(SelectedActions);
	check(SelectedActions.Num() == 1);
	check(SelectedActions[0]->GetTypeId() == FEdGraphSchemaAction_BlackboardEntry::StaticGetTypeId());
	TSharedPtr<FEdGraphSchemaAction_BlackboardEntry> BlackboardEntryAction = StaticCastSharedPtr<FEdGraphSchemaAction_BlackboardEntry>(SelectedActions[0]);
	BlackboardEntryAction->bIsNew = true;

	GraphActionMenu->OnRequestRenameOnActionNode();
	*/
}

#undef LOCTEXT_NAMESPACE
