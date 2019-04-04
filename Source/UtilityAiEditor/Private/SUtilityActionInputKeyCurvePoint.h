#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "UtilityActionDetails.h"

DECLARE_DELEGATE_OneParam(FOnActionDelete, FGuid const &)

DECLARE_DELEGATE_TwoParams(FOnActionInputKeyCurvePointCommitted, FGuid const &, FUtilityActionInputCurveKey const &)

/**
* An input for a UtilityAction (configurable key and data for curve)
*/
class SUtilityActionInputKeyCurvePoint
	: public SCompoundWidget
{
	TSharedPtr<SNumericEntryBox<float>> mpWidgetInput;
	TSharedPtr<SNumericEntryBox<float>> mpWidgetOutput;

	FGuid mId;
	FUtilityActionInputCurveKey mData;

	FOnActionDelete mOnDelete;
	FOnActionInputKeyCurvePointCommitted mOnValueCommitted;

	TOptional<float> GetValueInput() const;
	TOptional<float> GetValueOutput() const;
	void OnCommittedValueInput(float value, ETextCommit::Type type);
	void OnCommittedValueOutput(float value, ETextCommit::Type type);
	void OnRemovePressed();

public:

	SLATE_BEGIN_ARGS(SUtilityActionInputKeyCurvePoint)
		: _Id(FGuid())
		, _OnDelete(nullptr)
		, _OnValueCommitted(nullptr)
	{}

		SLATE_ATTRIBUTE(FGuid, Id)

		SLATE_EVENT(FOnActionDelete, OnDelete)

		SLATE_EVENT(FOnActionInputKeyCurvePointCommitted, OnValueCommitted)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};
