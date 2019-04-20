#include "UtilityTreeWizard.h"

// Window
#include "Interfaces/IMainFrameModule.h"
#include "Framework/Application/SlateApplication.h"

// Slate UI
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SSpacer.h"

#include "BehaviorTreeGraph.h"
#include "BehaviorTreeGraphNode.h"
#include "BehaviorTreeGraphNode_Composite.h"
#include "EdGraph/EdGraph.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "EdGraphSchema_BehaviorTree.h"

// Data
#include "SUtilityAction.h"

// Assets
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Engine/Classes/Curves/CurveFloat.h"
#include "UnrealEd/Classes/Factories/CurveFactory.h"
#include "AssetRegistryModule.h"

#include "BTComposite_UtilitySelector.h"
#include "BTComposite_UtilityNode.h"

#include "UtilityAiEditor.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_UtilityAiWizard"

//#define UTILITYAI_UPDATEDUE

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
			[//CreateVerticalBoxFillWidth(

				SNew(SVerticalBox)

					+SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[ CreateField(
						LOCTEXT("BlackboardAsset_Select", "Select Blackboard Asset"),
						SAssignNew(mpBlackboardSelector, SAssetSelectorBlackboardData)
						.OnAssetSelected(FOnAssetSelected::CreateRaw(this, &UtilityTreeWizard::OnBlackboardAssetSelected))
					) ]
	
					+SVerticalBox::Slot()
					.HAlign(HAlign_Fill)
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
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[
						SAssignNew(mpWidgetSwitcher, SWidgetSwitcher)
					]
				
			//)
			]
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
	this->mpBehaviorTreeAsset = MakeShareable(Cast<UBehaviorTree>(asset.GetAsset()));
	
	if (this->mpBehaviorTreeAsset->BlackboardAsset == nullptr)
	{
		return;
	}

	this->mUtilityTreeDetails.mpBlackboard = MakeShareable(this->mpBehaviorTreeAsset->BlackboardAsset);
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

// TAKEN FROM: SCurveEditor
UObject* CreateCurveObject(TSubclassOf<UCurveBase> CurveType, UObject* PackagePtr, FName& AssetName)
{
	UObject* NewObj = NULL;
	auto CurveFactory = Cast<UCurveFactory>(NewObject<UFactory>(GetTransientPackage(), UCurveFactory::StaticClass()));
	if (CurveFactory)
	{
		CurveFactory->CurveClass = CurveType;
		NewObj = CurveFactory->FactoryCreateNew(CurveFactory->GetSupportedClass(), PackagePtr, AssetName, RF_Public | RF_Standalone, NULL, GWarn);
	}
	CurveFactory = NULL;
	return NewObj;
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

	auto const actions = treeDetails.mActions;

	if (this->mpBehaviorTreeAsset->BlackboardAsset == nullptr)
	{
		return;
	}
	
	auto graphEd = this->mpBehaviorTreeAsset->BTGraph;

#ifdef UTILITYAI_UPDATEDUE
	UBehaviorTreeGraphNode_Composite* graphNode_selector;
	// Create the root selector node
	{
		FGraphNodeCreator<UBehaviorTreeGraphNode_Composite> NodeBuilder(*graphEd);
		graphNode_selector = NodeBuilder.CreateNode();

		graphNode_selector->NodePosX = 50;
		graphNode_selector->NodePosY = 50;

		auto utilityNode_selector = NewObject<UBTComposite_UtilitySelector>();
		utilityNode_selector->NodeName = TEXT("Utility Action Selector");
		graphNode_selector->NodeInstance = utilityNode_selector;

		NodeBuilder.Finalize();
	}
#endif

	UE_LOG(LogUtilityAiEditor, Log, TEXT("Creating utility tree in BT %s with blackboard %s"),
		*this->mpBehaviorTreeAsset->GetName(),
		*this->mpBehaviorTreeAsset->BlackboardAsset->GetName());

	auto assetPackage = this->mpBehaviorTreeAsset->GetOutermost();

	TArray<UBehaviorTreeGraphNode_Composite*> utilityActionNodes;
	for (auto const action : actions)
	{
		FName const actionName = action.mName;
		UE_LOG(LogUtilityAiEditor, Log, TEXT("Found action: %s"), *actionName.ToString());

#ifdef UTILITYAI_UPDATEDUE
		// Create the graph node
		FGraphNodeCreator<UBehaviorTreeGraphNode_Composite> NodeBuilder(*graphEd);
		auto graphNode_composite = NodeBuilder.CreateNode();
		graphNode_composite->NodePosX = 50;
		graphNode_composite->NodePosY = 50;
		
		// Create the utility action node instance
		auto utilityNode_action = NewObject<UBTComposite_UtilityNode>();
		utilityNode_action->NodeName = actionName.ToString();

		// Create the array of inputs for the action
		utilityNode_action->Inputs.Empty();
#endif

		// Iterate through all inputs and add them to the node
		for (auto const actionInputPair : action.mInputs)
		{
			auto const actionInputValue = actionInputPair.Value;
			UE_LOG(LogUtilityAiEditor, Log, TEXT("%s|%s"), *actionName.ToString(),
				*actionInputValue.mBlackboardKeyEntry.mName.ToString()
			);

			// Create the input that will be saved into the node
			auto actionInput = FUtilityInput();
			// Create the blackboard key selector
			actionInput.Key = FBlackboardKeySelector();
			actionInput.Key.SelectedKeyName = actionInputValue.mBlackboardKeyEntry.mName;

			// Create the curve asset for the values in this input
			FName curveAssetFileName = FName(*(this->mpBehaviorTreeAsset->BlackboardAsset->GetName() + TEXT("_") + actionName.ToString()));
			auto curveAsset = Cast<UCurveFloat>(CreateCurveObject(UCurveFloat::StaticClass(), assetPackage, curveAssetFileName));
			if (!curveAsset)
			{
				UE_LOG(LogUtilityAiEditor, Log, TEXT("Could not generate curve asset %s for action %s."), *curveAssetFileName.ToString(), *actionName.ToString());
				continue;
			}

			// Load points into the asset
			auto curvePointMap = actionInputValue.mCurveKeys;

			TArray<FUtilityActionInputCurveKey> curveKeys;
			curvePointMap.GenerateValueArray(curveKeys);
			for (auto const curveKey : curveKeys)
			{
				auto handle = curveAsset->FloatCurve.AddKey(curveKey.input, curveKey.output);
				curveAsset->FloatCurve.Keys[curveAsset->FloatCurve.GetIndexSafe(handle)].InterpMode = ERichCurveInterpMode::RCIM_Cubic;
			}

			// Notify the asset registry
			FAssetRegistryModule::AssetCreated(curveAsset);

			// Link the asset into the node
			actionInput.Curve = curveAsset;

#ifdef UTILITYAI_UPDATEDUE
			// Save the blackboard key / curve asset pair into the node
			utilityNode_action->Inputs.Add(actionInput);
#endif
		}

#ifdef UTILITYAI_UPDATEDUE
		// Update graph
		graphNode_composite->NodeInstance = utilityNode_action;
		NodeBuilder.Finalize();
		
		// Link utility action node with its parent (the selector node)
		graphNode_selector->AddSubNode(graphNode_composite, graphEd);
#endif
	}

	// Mark the package dirty...
	assetPackage->MarkPackageDirty();

	//IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
	//IMainFrameModule& MainFrameModule = IMainFrameModule::Get();
	//FContentBrowserModule &ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	//IContentBrowserSingleton &browser = ContentBrowserModule.Get();
	//TSharedPtr<UCurveFloatFactory> factory = MakeShareable(NewObject<UCurveFloatFactory>());
	//factory->FactoryCreateNew(UCurveFloat::StaticClass(), nullptr, "TestCurve", EObjectFlags::RF_NoFlags, nullptr, );
	//browser.CreateNewAsset();

	//FContentBr

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

	this->mUtilityTreeDetails.mActions.Empty();
}

void UtilityTreeWizard::OnUserScrolled(float amount)
{

}

#undef LOCTEXT_NAMESPACE

#ifdef UTILITYAI_UPDATEDUE
#undef UTILITYAI_UPDATEDUE
#endif
