#include "UtilityTreeWizard.h"

#include "IMainFrameModule.h"
#include "SlateApplication.h"

#include "Widgets/SWindow.h"
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
		SNew(SWindow)
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
	this->mUtilityTreeDetails.mpBlackboard = Cast<UBlackboardData>(asset.GetAsset());
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
	];
	this->mpWidgetSwitcher->SetActiveWidgetIndex(nextIndex);
	
	this->mUtilityTreeDetails.mActions.Add(action);

	/*
	SNew(SVerticalBox)

	// Blackboard Selector Asset
	+ SVerticalBox::Slot()
	.HAlign(HAlign_Center)
	.VAlign(VAlign_Top)
	.AutoHeight()
	[
	CreateField(
	LOCTEXT("BlackboardAsset_Select", "Select Blackboard Asset"),
	SNew(SComboButton)
	.OnGetMenuContent(FOnGetContent::CreateRaw(this, &UtilityTreeWizard::BuildMenuSelectBlackboardAsset))
	.ContentPadding(FMargin(2.0f, 2.0f))
	.MenuPlacement(MenuPlacement_BelowAnchor)
	.ButtonContent()
	[
	SAssignNew(mWidgetBlackboardAssetName, STextBlock)
	.Text(LOCTEXT("BlackboardAsset_Select", "Select Blackboard Asset"))
	]
	)
	]
	//*/
}

void UtilityTreeWizard::GenerateNodes()
{
}

void UtilityTreeWizard::OnUserScrolled(float amount)
{

}

#undef LOCTEXT_NAMESPACE
