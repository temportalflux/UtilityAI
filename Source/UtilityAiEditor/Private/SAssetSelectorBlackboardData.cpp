#include "SAssetSelectorBlackboardData.h"
#include "UtilityAiEditor.h"

#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_AssetSelectorBlackboardData"

const FText SAssetSelectorBlackboardData::TextSelectionPrompt = LOCTEXT("AssetSelectorBlackboardData_Prompt", "Select Asset");
const FText SAssetSelectorBlackboardData::TextSelectionConfirm = LOCTEXT("AssetSelectorBlackboardData_Confirm", "Confirm");

void SAssetSelectorBlackboardData::GetAllowedClasses(TArray<const UClass*>& classes)
{
	classes = this->mClasses;
}

void SAssetSelectorBlackboardData::OnSelectedAsset(const FAssetData& AssetData)
{
	//this->mpAsset = Cast<UBlackboardData>(AssetData.GetAsset());
	this->mpAsset = Cast<UBehaviorTree>(AssetData.GetAsset());

	this->mpWidgetAssetName->SetText(
		this->mpAsset != nullptr
		? FText::FromString(this->mpAsset->GetName())
		: TextSelectionPrompt
	);
	this->mpButtonConfirm->SetEnabled(this->mpAsset != nullptr);
}

void SAssetSelectorBlackboardData::CloseMenuAsset()
{
	FSlateApplication::Get().DismissAllMenus();
}

void SAssetSelectorBlackboardData::OnConfirm()
{
	this->mpButtonSelector->SetEnabled(false);
	this->mpButtonConfirm->SetEnabled(false);
	this->mpOnAssetConfirmed.ExecuteIfBound(FAssetData(this->mpAsset));
}

TSharedRef<SWidget> SAssetSelectorBlackboardData::BuildSelectMenu()
{
	return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		FAssetData(this->mpAsset),
		true,
		this->mClasses,
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(this->mClasses),
		FOnShouldFilterAsset(),
		FOnAssetSelected::CreateRaw(this, &SAssetSelectorBlackboardData::OnSelectedAsset),
		FSimpleDelegate::CreateRaw(this, &SAssetSelectorBlackboardData::CloseMenuAsset)
	);
}

void SAssetSelectorBlackboardData::Construct(const FArguments& InArgs)
{
	this->mpOnAssetConfirmed = InArgs._OnAssetSelected;
	this->mpAsset = nullptr;

	this->mClasses.Empty();
	// TODO: Rename this widget, its actually a behaviortree selector
	//this->mClasses.Add(UBlackboardData::StaticClass());
	this->mClasses.Add(UBehaviorTree::StaticClass());

	ChildSlot
	[
		SNew(SHorizontalBox)

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SAssignNew(mpButtonSelector, SComboButton)
				.OnGetMenuContent(FOnGetContent::CreateRaw(this, &SAssetSelectorBlackboardData::BuildSelectMenu))
				.ContentPadding(FMargin(2.0f, 2.0f))
				.MenuPlacement(MenuPlacement_BelowAnchor)
				.ButtonContent()
				[
					SAssignNew(mpWidgetAssetName, STextBlock)
					.Text(TextSelectionPrompt)
				]
			]

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[SNew(SSpacer)]

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SAssignNew(mpButtonConfirm, SButton)
				.IsEnabled(false)
				.OnPressed(FSimpleDelegate::CreateRaw(this, &SAssetSelectorBlackboardData::OnConfirm))
				.Content()[SNew(STextBlock).Text(TextSelectionConfirm)]
			]

	];
}

#undef LOCTEXT_NAMESPACE
