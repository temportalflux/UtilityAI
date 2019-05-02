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
	TWeakPtr<FUtilityActionDetails> mpDetails;

	int32 mIndex;
	UBlackboardData* mpBlackboard;

	TSharedPtr<SVerticalBox> mpActionInputsBox;
	TMap<FGuid, TSharedPtr<SWidget>> mInputWidgets;

	FSimpleDelegate mfOnDelete;

	void OnNameCommitted(FText const &text, ETextCommit::Type commitType);
	void AddInputField();
	void RemoveInputField(FGuid const &id);
	void OnInputValueCommitted(FGuid const &id, FUtilityActionInput const &data);
	void OnPressedDelete();

public:

	SLATE_BEGIN_ARGS(SUtilityAction)
		: _Value(nullptr)
		, _BlackboardAsset(nullptr)
		, _OnDelete()
	{}

		SLATE_ATTRIBUTE(TWeakPtr<FUtilityActionDetails>, Value)
		SLATE_ATTRIBUTE(UBlackboardData*, BlackboardAsset)
		SLATE_EVENT(FSimpleDelegate, OnDelete)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
