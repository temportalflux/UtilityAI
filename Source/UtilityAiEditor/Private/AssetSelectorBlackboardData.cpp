#include "AssetSelectorBlackboardData.h"
#include "UtilityAiEditor.h"

#include "SlateApplication.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_AssetSelectorBlackboardData"

const FText AssetSelectorBlackboardData::TextSelectionPrompt = LOCTEXT("AssetSelectorBlackboardData_Prompt", "Select Asset");
const FText AssetSelectorBlackboardData::TextSelectionConfirm = LOCTEXT("AssetSelectorBlackboardData_Confirm", "Confirm");

void AssetSelectorBlackboardData::GetAllowedClasses(TArray<const UClass*>& classes)
{
	classes = this->mClasses;
}

void AssetSelectorBlackboardData::OnSelectedAsset(const FAssetData& AssetData)
{
	this->mpAsset = Cast<UBlackboardData>(AssetData.GetAsset());

	this->mpWidgetAssetName->SetText(
		this->mpAsset != nullptr
		? FText::FromString(this->mpAsset->GetName())
		: TextSelectionPrompt
	);
	this->mpButtonConfirm->SetEnabled(this->mpAsset != nullptr);
}

void AssetSelectorBlackboardData::CloseMenuAsset()
{
	FSlateApplication::Get().DismissAllMenus();
}

void AssetSelectorBlackboardData::OnConfirm()
{
	this->mpButtonSelector->SetEnabled(false);
	this->mpButtonConfirm->SetEnabled(false);
	this->mpOnAssetConfirmed.ExecuteIfBound(FAssetData(this->mpAsset));
}

TSharedRef<SWidget> AssetSelectorBlackboardData::BuildSelectMenu()
{
	return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		FAssetData(this->mpAsset),
		true,
		this->mClasses,
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(this->mClasses),
		FOnShouldFilterAsset(),
		FOnAssetSelected::CreateRaw(this, &AssetSelectorBlackboardData::OnSelectedAsset),
		FSimpleDelegate::CreateRaw(this, &AssetSelectorBlackboardData::CloseMenuAsset)
	);
}

TSharedRef<SWidget> AssetSelectorBlackboardData::Construct(FOnAssetSelected onAssetConfirmed)
{
	this->mpOnAssetConfirmed = onAssetConfirmed;
	this->mpAsset = nullptr;

	this->mClasses.Empty();
	this->mClasses.Add(UBlackboardData::StaticClass());

	return SNew(SHorizontalBox)

		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SAssignNew(mpButtonSelector, SComboButton)
			.OnGetMenuContent(FOnGetContent::CreateRaw(this, &AssetSelectorBlackboardData::BuildSelectMenu))
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
			.OnPressed(FSimpleDelegate::CreateRaw(this, &AssetSelectorBlackboardData::OnConfirm))
			.Content()[SNew(STextBlock).Text(TextSelectionConfirm)]
		]

	;
}

#undef LOCTEXT_NAMESPACE
