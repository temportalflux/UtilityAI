#include "UtilityTreeWizard.h"

#include "IMainFrameModule.h"
#include "SlateApplication.h"

#include "Widgets/SWindow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "PropertyCustomizationHelpers.h"

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

void SelectorBlackboardData::GetAllowedClasses(TArray<const UClass*>& classes)
{
	classes = this->mClasses;
}

void SelectorBlackboardData::OnSelectedAsset(const FAssetData& AssetData)
{
	this->mpAsset = Cast<UBlackboardData>(AssetData.GetAsset());

	this->mpWidgetAssetName->SetText(
		this->mpAsset != nullptr
		? FText::FromString(this->mpAsset->GetName())
		: LOCTEXT("SelectorBlackboardData_Prompt", "Select Asset")
	);
}

void SelectorBlackboardData::CloseMenuAsset()
{
	FSlateApplication::Get().DismissAllMenus();
}

TSharedRef<SWidget> SelectorBlackboardData::Construct()
{
	this->mClasses.Empty();
	this->mClasses.Add(UBlackboardData::StaticClass());

	FAssetData CurrentAssetData = FAssetData(this->mpAsset);
	return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		CurrentAssetData,
		true,
		this->mClasses,
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(this->mClasses),
		FOnShouldFilterAsset(),
		FOnAssetSelected::CreateRaw(this, &SelectorBlackboardData::OnSelectedAsset),
		FSimpleDelegate::CreateRaw(this, &SelectorBlackboardData::CloseMenuAsset)
	);
}

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
			[ CreateVerticalBoxFillWidth(
				SAssignNew(mpWidgetSwitcher, SWidgetSwitcher)
				/*
				SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					[
						SNew(STextBlock)
						.Text(FText::FromString(TEXT("Create Action")))
					]
				
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

	mpWidgetSwitcher->SetActiveWidgetIndex(INDEX_NONE);
}

void UtilityTreeWizard::OnUserScrolled(float amount)
{

}

TSharedRef<SWidget> UtilityTreeWizard::BuildMenuSelectBlackboardAsset()
{
	return this->mpBlackboardSelector->Construct();
}

#undef LOCTEXT_NAMESPACE
