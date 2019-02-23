#pragma once

#include "CoreMinimal.h"
#include "SlateFwd.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

#include "BehaviorTree/BlackboardData.h"
#include "Widgets/Text/STextBlock.h"

DECLARE_DELEGATE_OneParam(FOnSelectedEntry, FBlackboardEntry)
DECLARE_DELEGATE(FOnClearSelection)

class SSelectorBlackboardEntry
	: public SCompoundWidget
{
	TSharedPtr<UBlackboardData> mpBlackboard;

	TSharedPtr<STextBlock> mpLabel;

	FOnSelectedEntry mOnSelection;
	FOnClearSelection mOnClearSelection;

	TSharedRef<SWidget> BuildSelectMenu();
	void OnClear();

public:

	SLATE_BEGIN_ARGS(SSelectorBlackboardEntry)
		: _BlackboardAsset(nullptr)
		, _OnSelection()
		, _OnClearSelection()
	{}

		SLATE_ATTRIBUTE(TSharedPtr<UBlackboardData>, BlackboardAsset)

		SLATE_EVENT(FOnSelectedEntry, OnSelection)
		SLATE_EVENT(FOnClearSelection, OnClearSelection)

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

};

