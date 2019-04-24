#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "UtilityActionDetails.h"

DECLARE_DELEGATE_TwoParams(FOnUtilityActionChanged, int32 const &, FUtilityActionDetails const &)

/**
* All configurable data about a specific action
*/
class SUtilityAction
	: public SCompoundWidget
{
	static const FText TextActionNameLabel;

	FUtilityActionDetails mDetails;

	int32 mIndex;
	TSharedPtr<UBlackboardData> mpBlackboard;

	TSharedPtr<SVerticalBox> mpActionInputsBox;
	TMap<FGuid, TSharedPtr<SWidget>> mInputWidgets;

	FOnUtilityActionChanged mOnChanged;

	void OnNameCommitted(FText const &text, ETextCommit::Type commitType);
	void AddInputField();
	void RemoveInputField(FGuid const &id);
	void OnInputValueCommitted(FGuid const &id, FUtilityActionInput const &data);

public:

	SLATE_BEGIN_ARGS(SUtilityAction)
		: _Index(0)
		, _BlackboardAsset(nullptr)
		, _OnChanged()
	{}

		SLATE_ATTRIBUTE(int32, Index)
		SLATE_ATTRIBUTE(TSharedPtr<UBlackboardData>, BlackboardAsset)

		SLATE_EVENT(FOnUtilityActionChanged, OnChanged)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
