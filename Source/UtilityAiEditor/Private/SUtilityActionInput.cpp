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

	// TODO: Utility Value (probablity)
	//		low (25%), medium (50%), high (75%), definite (500%)
	// TODO: Curve Type
	//		constant, smooth, ???
	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
			
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoWidth()
					[
						SNew(STextBlock)
						.Text(TextLabelBlackboardKey)
					]
			
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoWidth()
					[
						SNew(SButton)
						.Text(LOCTEXT("DeleteEntry", "Remove"))
						.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityActionInput::OnDelete))
					]
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SVerticalBox)

					+SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[
						SNew(SBlackboardEntryInfo)
						.BlackboardAsset(this->mpBlackboard)
						.OnChanged(FOnBlackboardEntryChanged::CreateRaw(this, &SUtilityActionInput::OnChangedBlackboardKey))
					]
			
					+SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[
						SNew(SButton)
						.Text(LOCTEXT("AddCurveKey", "Add Curve Key"))
						.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityActionInput::AddCurveKey))
					]

					+SVerticalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Top)
					.AutoHeight()
					[
						SAssignNew(mpWidgetBoxCurveKeys, SVerticalBox)
					]
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
	this->mData.mCurveKeys[id] = value;
	this->mOnValueCommitted.ExecuteIfBound(this->mId, this->mData);
}

#undef LOCTEXT_NAMESPACE 
