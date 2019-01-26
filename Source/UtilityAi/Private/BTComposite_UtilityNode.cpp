// Fill out your copyright notice in the Description page of Project Settings.

#include "BTComposite_UtilityNode.h"
#include "Math.h"
#include "UtilityAi.h"

UBTComposite_UtilityNode::UBTComposite_UtilityNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Utility Node";
	bUseNodeActivationNotify = true;

	OnNextChild.BindUObject(this, &UBTComposite_UtilityNode::GetNextChildHandler);
}

FString UBTComposite_UtilityNode::GetStaticDescription() const
{
	return TEXT("TODO: Description of node");
}

#if WITH_EDITOR

FName UBTComposite_UtilityNode::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Sequence.Icon");
}

#endif

int32 UBTComposite_UtilityNode::EvaluateUtility() const
{
	return FMath::Rand() % this->UtilityValue;
	//return this->UtilityValue;
}

int32 UBTComposite_UtilityNode::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	// failure = quit
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		// newly activated: start from first
		NextChildIdx = 0;
	}
	else if (LastResult == EBTNodeResult::Succeeded && (PrevChild + 1) < this->GetChildrenNum())
	{
		// success = choose next child
		NextChildIdx = PrevChild + 1;
	}

	return NextChildIdx;
}
