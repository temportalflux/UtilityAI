#include "UtilityTreeWizard.h"

#include "IMainFrameModule.h"
#include "SlateApplication.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSpacer.h"

#include "SUtilityAction.h"

#include "UtilityAiEditor.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_UtilityAiWizard"

void OpenWindow(TSharedRef<SWindow> window)
{
	//IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	IMainFrameModule& MainFrameModule = IMainFrameModule::Get();

	if (MainFrameModule.GetParentWindow().IsValid())
	{
		FSlateApplication::Get().AddWindowAsNativeChild(window, MainFrameModule.GetParentWindow().ToSharedRef());
	}
	else
	{
		FSlateApplication::Get().AddWindow(window);
	}
}

TSharedRef<SWidget> CreateVerticalBoxFillWidth(TSharedRef<SWidget> child)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		.FillWidth(1.0f)
		[child];
}

TSharedRef<SWidget> CreateField(FText label, TSharedRef<SWidget> input)
{
	return SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(STextBlock)
			.Text(label)
		]

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[input]
	;
}

const FText UtilityTreeWizard::TextButtonFinish = LOCTEXT("UtilityTreeWizard_ButtonFinish", "Finish");
const FText UtilityTreeWizard::TextButtonAddAction = LOCTEXT("UtilityTreeWizard_ButtonAddAction", "Add Action");

void UtilityTreeWizard::Open()
{
	this->mUtilityTreeDetails.mpBlackboard = 0;

	OpenWindow(
		SAssignNew(mpWindow, SWindow)
		.Title(FText::FromString(TEXT("Create Utility Tree")))
		.ClientSize(FVector2D(400, 800))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
			SNew(SScrollBox)
			.Orientation(Orient_Vertical)
			.ScrollBarAlwaysVisible(true)
			.OnUserScrolled(FOnUserScrolled::CreateRaw(this, &UtilityTreeWizard::OnUserScrolled))
			+ SScrollBox::Slot()
			[CreateVerticalBoxFillWidth(

				SNew(SVerticalBox)

					+SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[ CreateField(
						LOCTEXT("BlackboardAsset_Select", "Select Blackboard Asset"),
						SAssignNew(mpBlackboardSelector, SAssetSelectorBlackboardData)
						.OnAssetSelected(FOnAssetSelected::CreateRaw(this, &UtilityTreeWizard::OnBlackboardAssetSelected))
					) ]
	
					+SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[
						SNew(SHorizontalBox)

							+SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.VAlign(VAlign_Center)
							.AutoWidth()
							[
								SAssignNew(mpButtonFinish, SButton)
								.IsEnabled(false)
								.OnPressed(FSimpleDelegate::CreateRaw(this, &UtilityTreeWizard::GenerateNodes))
								.Content()
								[ SNew(STextBlock).Text(TextButtonFinish) ]
							]

							+SHorizontalBox::Slot()
							.HAlign(HAlign_Left)
							.VAlign(VAlign_Center)
							.AutoWidth()
							[
								SAssignNew(mpButtonAddAction, SButton)
								.IsEnabled(false)
								.OnPressed(FSimpleDelegate::CreateRaw(this, &UtilityTreeWizard::AddAction))
								.Content()
								[ SNew(STextBlock).Text(TextButtonAddAction) ]
							]

					]

					+SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[
						SAssignNew(mpWidgetSwitcher, SWidgetSwitcher)
					]
				
			) ]
			/*
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
				[
					SNew(SComboBox<TSharedPtr<FComboTest>>)
						.OptionsSource(&Options)
						.OnSelectionChanged_Lambda(
							[&](TSharedPtr<FComboTest> Item, ESelectInfo::Type) {
								CurrentItem = Item;
								UpdateProperty();
							}
						)
						.OnGenerateWidget_Lambda(
							[&](TSharedPtr<FComboTest> Item) {
								return SNew(STextBlock)
									.Font(IDetailLayoutBuilder::GetDetailFont())
									.Text(Item->Label);
							}
						)
						.InitiallySelectedItem(CurrentItem)
						[
							SAssignNew(CurrentText, STextBlock)
								.Font(IDetailLayoutBuilder::GetDetailFont())
								.Text(CurrentItem->Label)
						]
				]
				//*/
		]
	);

	this->mpWidgetSwitcher->SetActiveWidgetIndex(INDEX_NONE);
}

void UtilityTreeWizard::OnBlackboardAssetSelected(FAssetData const &asset)
{
	this->mUtilityTreeDetails.mpBlackboard = MakeShareable(Cast<UBlackboardData>(asset.GetAsset()));
	this->mpButtonFinish->SetEnabled(true);
	this->mpButtonAddAction->SetEnabled(true);
	this->AddAction();
}

void UtilityTreeWizard::AddAction()
{
	FUtilityActionDetails action;

	int32 nextIndex = this->mpWidgetSwitcher->GetActiveWidgetIndex() + 1;
	this->mpWidgetSwitcher->AddSlot()
	[
		SNew(SUtilityAction)
		.Index(nextIndex)
		.BlackboardAsset(this->mUtilityTreeDetails.mpBlackboard)
		.OnChanged(FOnUtilityActionChanged::CreateRaw(this, &UtilityTreeWizard::OnActionChanged))
	];
	this->mpWidgetSwitcher->SetActiveWidgetIndex(nextIndex);
	
	this->mUtilityTreeDetails.mActions.Add(action);
}

void UtilityTreeWizard::OnActionChanged(int32 const &index, FUtilityActionDetails const &data)
{
	this->mUtilityTreeDetails.mActions[index] = data;
}

void UtilityTreeWizard::GenerateNodes()
{
	if (this->mpWindow.IsValid())
	{
		this->mpWindow->RequestDestroyWindow();
		this->mpWindow = nullptr;
	}
	else return;

	FUtilityTreeDetails const &treeDetails = this->mUtilityTreeDetails;

	auto const pBlackboard = treeDetails.mpBlackboard;

	if (!pBlackboard.IsValid()) return;

	auto const actions = treeDetails.mActions;

	UE_LOG(LogUtilityAiEditor, Log, TEXT("Found utility tree with blackboard %s"), *pBlackboard->GetName());
	for (auto const action : actions)
	{
		FName const actionName = action.mName;
		UE_LOG(LogUtilityAiEditor, Log, TEXT("Found action: %s"), *actionName.ToString());
	}

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

void UtilityTreeWizard::OnUserScrolled(float amount)
{

}

#undef LOCTEXT_NAMESPACE
