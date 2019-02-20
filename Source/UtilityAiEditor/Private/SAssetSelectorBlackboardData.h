#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/SWidget.h"

#include "BehaviorTree/BlackboardData.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Text/STextBlock.h"

#include "PropertyCustomizationHelpers.h"

class SAssetSelectorBlackboardData
	: public SCompoundWidget
{
	static const FText TextSelectionPrompt;
	static const FText TextSelectionConfirm;

	UBlackboardData *mpAsset;
	FOnAssetSelected mpOnAssetConfirmed;

	TArray<const UClass*> mClasses;
	TSharedPtr<STextBlock> mpWidgetAssetName;
	TSharedPtr<SComboButton> mpButtonSelector;
	TSharedPtr<SButton> mpButtonConfirm;

	void GetAllowedClasses(TArray<const UClass*>& classes);
	TSharedRef<SWidget> BuildSelectMenu();
	void OnSelectedAsset(const FAssetData& AssetData);
	void CloseMenuAsset();
	void OnConfirm();

public:

	SLATE_BEGIN_ARGS(SAssetSelectorBlackboardData)
		: _OnAssetSelected(0)
	{}

		/** Exposes a delegate to be invoked when the wizard is canceled. */
		SLATE_EVENT(FOnAssetSelected, OnAssetSelected)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
