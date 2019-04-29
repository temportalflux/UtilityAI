#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

#include "BehaviorTree/BlackboardData.h"
#include "UtilityActionEntry.h"
#include "Widgets/SBoxPanel.h"

DECLARE_DELEGATE_OneParam(FOnBlackboardEntryChanged, const FUtilityActionEntry&);

class SBlackboardEntryInfo
	: public SCompoundWidget
{
	UBlackboardData* mpBlackboard;
	FName mInputtedEntryName;
	UBlackboardKeyType* mpInputtedEntryType;
	FUtilityActionEntry mEntry;

	FOnBlackboardEntryChanged mOnChanged;

	TSharedPtr<SVerticalBox> mpWidgetEntryVars;

	void OnClearEntrySelection();
	void OnMakeEntrySelection(FBlackboardEntry entry);

	void OnEntryNameCommitted(FText const &text, ETextCommit::Type commitPhase);
	void OnSelectKeyType(UClass* InClass);

public:

	SLATE_BEGIN_ARGS(SBlackboardEntryInfo)
		: _BlackboardAsset(nullptr)
		, _OnChanged(nullptr)
	{}

		SLATE_ATTRIBUTE(UBlackboardData*, BlackboardAsset)

		SLATE_EVENT(FOnBlackboardEntryChanged, OnChanged)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};

