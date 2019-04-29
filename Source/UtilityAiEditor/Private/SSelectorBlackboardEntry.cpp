#include "SSelectorBlackboardEntry.h"

#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SComboButton.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "SSelectorBlackboardVarType.h"
#include "UtilityAiEditor.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SSelectorBlackboardEntry"

TSharedRef<SWidget> SSelectorBlackboardEntry::BuildSelectMenu()
{
	FMenuBuilder menuBuilder(true, nullptr, nullptr, true);

	if (this->mpBlackboard == nullptr) return menuBuilder.MakeWidget();

	FOnSelectedEntry onExecute = mOnSelection;
	TSharedPtr<STextBlock> dropdownLabel = mpLabel;

	menuBuilder.AddMenuEntry(
		LOCTEXT("Prompt", "Create New Entry"),
		LOCTEXT("PromptDesc", "Create a new blackboard key"),
		FSlateIcon(),
		FUIAction(FExecuteAction::CreateRaw(this, &SSelectorBlackboardEntry::OnClear))
	);

#if WITH_EDITORONLY_DATA
	for (FBlackboardEntry entry : this->mpBlackboard->ParentKeys)
	{
		menuBuilder.AddMenuEntry(
			FText::FromName(entry.EntryName),
			FText::FromString(entry.EntryDescription),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateLambda(
				[dropdownLabel, entry, onExecute]()
				{
					dropdownLabel->SetText(FText::FromName(entry.EntryName));
					onExecute.ExecuteIfBound(entry);
				}
			))
		);
	}
#endif

	for (FBlackboardEntry entry : this->mpBlackboard->Keys)
	{
		menuBuilder.AddMenuEntry(
			FText::FromName(entry.EntryName),
			FText::FromString(entry.EntryDescription),
			FSlateIcon(),
			FUIAction(FExecuteAction::CreateLambda(
				[dropdownLabel, entry, onExecute]()
				{
					dropdownLabel->SetText(FText::FromName(entry.EntryName));
					onExecute.ExecuteIfBound(entry);
				}
			))
		);
	}

	return menuBuilder.MakeWidget();
}

void SSelectorBlackboardEntry::Construct(const FArguments & InArgs)
{
	this->mpBlackboard = InArgs._BlackboardAsset.Get();
	this->mOnSelection = InArgs._OnSelection;
	this->mOnClearSelection = InArgs._OnClearSelection;

	ChildSlot
	[
		SNew(SComboButton)
		.OnGetMenuContent(FOnGetContent::CreateRaw(this,
			&SSelectorBlackboardEntry::BuildSelectMenu))
		.ContentPadding(FMargin(2.0f, 2.0f))
		.MenuPlacement(MenuPlacement_BelowAnchor)
		.ButtonContent()
		[
			SAssignNew(mpLabel, STextBlock)
		]
	];

	OnClear();
}

void SSelectorBlackboardEntry::OnClear()
{
	mpLabel->SetText(LOCTEXT("Label", "Create New Key"));
	mOnClearSelection.ExecuteIfBound();
}

#undef LOCTEXT_NAMESPACE
