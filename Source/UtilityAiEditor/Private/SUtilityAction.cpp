#include "SUtilityAction.h"
#include "UtilityAiEditor.h"

#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "SUtilityActionInput.h"

#define LOCTEXT_NAMESPACE "UtilityAiEditor_SUtilityAction"

const FText SUtilityAction::TextActionNameLabel = LOCTEXT("UtilityTreeWizard_CreateAction", "Action Name");

void SUtilityAction::Construct(const FArguments& InArgs)
{
	this->mIndex = InArgs._Index.Get();
	this->mpBlackboard = InArgs._BlackboardAsset.Get();
	this->mOnChanged = InArgs._OnChanged;

	int32 index = InArgs._Index.Get();

	this->mDetails.mName = FName(*FText::FormatNamed(FText::FromString(TEXT("Action {index}")), TEXT("index"), index).ToString());

	ChildSlot
	[
		SNew(SVerticalBox)

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(STextBlock).Text(SUtilityAction::TextActionNameLabel)
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SEditableTextBox)
						.Text(FText::FromName(this->mDetails.mName))
						.OnTextCommitted(FOnTextCommitted::CreateRaw(this, &SUtilityAction::OnNameCommitted))
					]
			]
	
			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("AddEntry", "Create input"))
				.OnPressed(FSimpleDelegate::CreateRaw(this, &SUtilityAction::AddInputField))
			]

			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SAssignNew(mpActionInputsBox, SVerticalBox)
			]

	];
}

void SUtilityAction::OnNameCommitted(FText const &text, ETextCommit::Type commitType)
{
	this->mDetails.mName = FName(*text.ToString());
	this->mOnChanged.ExecuteIfBound(this->mIndex, this->mDetails);
}

void SUtilityAction::AddInputField()
{
	FGuid id = FGuid::NewGuid();
	
	this->mDetails.mInputs.Add(id, FUtilityActionInput());

	TSharedPtr<SWidget> tmp;
	this->mpActionInputsBox->AddSlot()
		.AutoHeight()
	[
		SAssignNew(tmp, SUtilityActionInput)
		.Id(id)
		.BlackboardAsset(this->mpBlackboard)
		.OnDelete(FOnActionDelete::CreateRaw(this, &SUtilityAction::RemoveInputField))
		.OnValueCommitted(FOnActionInputCommitted::CreateRaw(this, &SUtilityAction::OnInputValueCommitted))
	];

	mInputWidgets.Add(id, tmp);

	this->mOnChanged.ExecuteIfBound(this->mIndex, this->mDetails);
}

void SUtilityAction::RemoveInputField(FGuid const &id)
{
	this->mpActionInputsBox->RemoveSlot(this->mInputWidgets[id].ToSharedRef());
	this->mInputWidgets.Remove(id);
	this->mDetails.mInputs.Remove(id);

	this->mOnChanged.ExecuteIfBound(this->mIndex, this->mDetails);
}

void SUtilityAction::OnInputValueCommitted(FGuid const &id, FUtilityActionInput const &data)
{
	this->mDetails.mInputs[id] = data;
	this->mOnChanged.ExecuteIfBound(this->mIndex, this->mDetails);
}

#undef LOCTEXT_NAMESPACE