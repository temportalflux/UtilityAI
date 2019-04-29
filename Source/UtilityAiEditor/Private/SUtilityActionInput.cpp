#include "SUtilityActionInput.h"

#include "Widgets/SBoxPanel.h"
#include "SBlackboardEntryInfo.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SUtilityActionInput"

const FText SUtilityActionInput::TextLabelBlackboardKey = LOCTEXT("BlackboardKey", "Blackboard Key");

void SUtilityActionInput::Construct(const FArguments & InArgs)
{
	this->mId = InArgs._Id.Get();
	this->mpBlackboard = InArgs._BlackboardAsset.Get();
	this->mOnDelete = InArgs._OnDelete;
	this->mOnValueCommitted = InArgs._OnValueCommitted;
	this->mpWidgetCurveKeys.Empty();

	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("DeleteEntry", "Remove Input"))
				.ButtonColorAndOpacity(FSlateColor(FLinearColor(0.8, 0, 0)))
				.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityActionInput::OnDelete))
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SBlackboardEntryInfo)
				.Text(LOCTEXT("BlackboardKey", "Blackboard Key"))
				.BlackboardAsset(this->mpBlackboard)
				.OnChanged(FOnBlackboardEntryChanged::CreateRaw(this, &SUtilityActionInput::OnChangedBlackboardKey))
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SHorizontalBox)

					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(5.0f)
					[
						SNew(STextBlock)
						.Text(LOCTEXT("CurveKeys", "Significant Values"))
					]

					+SHorizontalBox::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.AutoWidth()
					.Padding(5.0f)
					[
						SNew(SButton)
						.Text(LOCTEXT("AddCurveKey", "+ Point"))
						.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityActionInput::AddCurveKey))
					]

			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			.Padding(20.0f, 0, 0, 0)
			[
				SAssignNew(mpWidgetBoxCurveKeys, SVerticalBox)
			]

	];
}

void SUtilityActionInput::OnDelete()
{
	this->mOnDelete.ExecuteIfBound(this->mId);
}

void SUtilityActionInput::AddCurveKey()
{
	FGuid id = FGuid::NewGuid();
	this->mData.mCurveKeys.Add(id, FUtilityActionInputCurveKey());

	TSharedPtr<SUtilityActionInputKeyCurvePoint> widget;
	this->mpWidgetBoxCurveKeys->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.AutoHeight()
	[
		SAssignNew(widget, SUtilityActionInputKeyCurvePoint)
		.Id(id)
		.OnDelete(FOnActionDelete::CreateRaw(this, &SUtilityActionInput::RemoveCurveKey))
		.OnValueCommitted(FOnActionInputKeyCurvePointCommitted::CreateRaw(this, &SUtilityActionInput::OnValueCommittedCurveKey))
	];
	this->mpWidgetCurveKeys.Add(id, widget);

	this->mOnValueCommitted.ExecuteIfBound(this->mId, this->mData);
}

void SUtilityActionInput::RemoveCurveKey(FGuid const &id)
{
	this->mpWidgetBoxCurveKeys->RemoveSlot(this->mpWidgetCurveKeys[id].ToSharedRef());
	this->mpWidgetCurveKeys.Remove(id);
	this->mData.mCurveKeys.Remove(id);
	this->mOnValueCommitted.ExecuteIfBound(this->mId, this->mData);
}

void SUtilityActionInput::OnChangedBlackboardKey(FUtilityActionEntry const &key)
{
	mData.mBlackboardKeyEntry = key;
	this->mOnValueCommitted.ExecuteIfBound(this->mId, this->mData);
}

void SUtilityActionInput::OnValueCommittedCurveKey(FGuid const &id, FUtilityActionInputCurveKey const &value)
{
	if (!this->mData.mCurveKeys.Contains(id)) return;

	this->mData.mCurveKeys[id] = value;
	this->mOnValueCommitted.ExecuteIfBound(this->mId, this->mData);
}

#undef LOCTEXT_NAMESPACE 
