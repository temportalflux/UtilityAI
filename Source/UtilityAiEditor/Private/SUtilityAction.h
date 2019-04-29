#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SCompoundWidget.h"
#include "UtilityActionDetails.h"

/**
* All configurable data about a specific action
*/
class SUtilityAction
	: public SCompoundWidget
{
	static const FText TextActionNameLabel;

	TWeakPtr<FUtilityActionDetails> mpDetails;

	int32 mIndex;
	UBlackboardData* mpBlackboard;

	TSharedPtr<SVerticalBox> mpActionInputsBox;
	TMap<FGuid, TSharedPtr<SWidget>> mInputWidgets;

	void OnNameCommitted(FText const &text, ETextCommit::Type commitType);
	void AddInputField();
	void RemoveInputField(FGuid const &id);
	void OnInputValueCommitted(FGuid const &id, FUtilityActionInput const &data);

public:

	SLATE_BEGIN_ARGS(SUtilityAction)
		: _BlackboardAsset(nullptr)
		, _Value(nullptr)
	{}

		SLATE_ATTRIBUTE(TWeakPtr<FUtilityActionDetails>, Value)
		SLATE_ATTRIBUTE(UBlackboardData*, BlackboardAsset)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
