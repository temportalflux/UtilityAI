#include "SBlackboardEntryInfo.h"

#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Input/SEditableTextBox.h"

#include "SSelectorBlackboardEntry.h"
#include "SSelectorBlackboardVarType.h"
#include "UtilityAiEditor.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SBlackboardEntryInfo"

void SBlackboardEntryInfo::Construct(const FArguments & InArgs)
{
	this->mpBlackboard = InArgs._BlackboardAsset.Get();
	this->mOnChanged = InArgs._OnChanged;
	mEntry.mShouldCreate = false;

	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

					// Label for main row
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(5.0f)
					[
						SNew(STextBlock)
						.Visibility(InArgs._Text.Get().IsEmptyOrWhitespace()
							? EVisibility::Hidden
							: EVisibility::Visible)
						.Text(InArgs._Text.Get())
					]

					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.FillWidth(1.0f)
					.Padding(5.0f)
					[
						SNew(SSelectorBlackboardEntry)
						.BlackboardAsset(this->mpBlackboard)
						.OnClearSelection(FOnClearSelection::CreateRaw(
							this, &SBlackboardEntryInfo::OnClearEntrySelection))
						.OnSelection(FOnSelectedEntry::CreateRaw(
							this, &SBlackboardEntryInfo::OnMakeEntrySelection))
					]

			]

			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			.Padding(20.0f, 0, 0, 0)
			[
				SAssignNew(mpWidgetEntryVars, SVerticalBox)
			]

	];
}

void SBlackboardEntryInfo::OnClearEntrySelection()
{
	if (mEntry.mShouldCreate) return;
	mEntry.mShouldCreate = true;
	mEntry.mName = this->mInputtedEntryName;
	mEntry.mpType = this->mpInputtedEntryType;
	
	this->mpWidgetEntryVars->AddSlot()
	[
		SNew(SHorizontalBox)

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.AutoWidth()
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("NewKeyName", "Key Name"))
			]

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.0f)
			.Padding(5.0f)
			[
				SNew(SEditableTextBox)
				.OnTextCommitted(FOnTextCommitted::CreateRaw(
					this, &SBlackboardEntryInfo::OnEntryNameCommitted
				))
			]

	];
	this->mpWidgetEntryVars->AddSlot()
	[
		SNew(SHorizontalBox)

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.AutoWidth()
			.Padding(5.0f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("NewKeyType", "Key Type"))
			]

			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillWidth(1.0f)
			.Padding(5.0f)
			[
				SNew(SSelectorBlackboardVarType)
				.OnSelected(FOnSelectedBlackboardVarType::CreateRaw(
					this, &SBlackboardEntryInfo::OnSelectKeyType
				))
			]
		
	];

	this->mOnChanged.ExecuteIfBound(this->mEntry);
}

void SBlackboardEntryInfo::OnMakeEntrySelection(FBlackboardEntry entry)
{
	if (mEntry.mShouldCreate)
	{
		this->mpWidgetEntryVars->ClearChildren();
	}

	mEntry.mShouldCreate = false;
	mEntry.mName = entry.EntryName;
	mEntry.mpType = entry.KeyType;
	
	this->mOnChanged.ExecuteIfBound(this->mEntry);
}

void SBlackboardEntryInfo::OnEntryNameCommitted(FText const &text, ETextCommit::Type commitPhase)
{
	mEntry.mName = mInputtedEntryName = FName(*text.ToString());
	this->mOnChanged.ExecuteIfBound(this->mEntry);
}

void SBlackboardEntryInfo::OnSelectKeyType(UClass* InClass)
{
	mEntry.mpType = this->mpInputtedEntryType = NewObject<UBlackboardKeyType>(mpBlackboard, InClass);
	this->mOnChanged.ExecuteIfBound(this->mEntry);
}

#undef LOCTEXT_NAMESPACE
