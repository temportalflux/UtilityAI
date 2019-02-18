#include "UtilityTreeWizard.h"

#include "IMainFrameModule.h"
#include "SlateApplication.h"

#include "Widgets/SWindow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBox.h"
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

TSharedRef<SWidget> CreateField(FText label, TSharedRef<SWidget> input)
{
	return SNew(SHorizontalBox)

		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
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

void UtilityTreeWizard::GetAllowedClasses(TArray<const UClass*>& classes)
{
	classes = this->mBlackboardClasses;
}

void UtilityTreeWizard::Open()
{
	this->mUtilityTreeDetails.mpBlackboard = 0;
	this->mBlackboardClasses.Empty();
	this->mBlackboardClasses.Add(UBlackboardData::StaticClass());

	OpenWindow(
		SNew(SWindow)
		.Title(FText::FromString(TEXT("Create Utility Tree")))
		.ClientSize(FVector2D(400, 800))
		.SupportsMaximize(false)
		.SupportsMinimize(false)
		[
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
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Top)
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
}

TSharedRef<SWidget> UtilityTreeWizard::BuildMenuSelectBlackboardAsset()
{
	FAssetData CurrentAssetData = FAssetData(this->mUtilityTreeDetails.mpBlackboard);
	return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		CurrentAssetData,
		true,
		this->mBlackboardClasses,
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(this->mBlackboardClasses),
		FOnShouldFilterAsset(),
		FOnAssetSelected::CreateRaw(this, &UtilityTreeWizard::OnSelectedBlackboardAsset),
		FSimpleDelegate::CreateRaw(this, &UtilityTreeWizard::CloseMenuBlackboardAsset)
	);
}

void UtilityTreeWizard::OnSelectedBlackboardAsset(const FAssetData& AssetData)
{
	this->mUtilityTreeDetails.mpBlackboard = Cast<UBlackboardData>(AssetData.GetAsset());
	
	this->mWidgetBlackboardAssetName->SetText(
		this->mUtilityTreeDetails.mpBlackboard != nullptr
		? FText::FromString(this->mUtilityTreeDetails.mpBlackboard->GetName())
		: LOCTEXT("BlackboardAsset_Select", "Select Blackboard Asset")
	);
}

void UtilityTreeWizard::CloseMenuBlackboardAsset()
{
	FSlateApplication::Get().DismissAllMenus();
}

#undef LOCTEXT_NAMESPACE
