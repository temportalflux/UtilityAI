// Fill out your copyright notice in the Description page of Project Settings.

#include "BTComposite_UtilitySelector.h"
#include "BTComposite_UtilityNode.h"
#include "UtilityAi.h"

UBTComposite_UtilitySelector::UBTComposite_UtilitySelector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Utility Selector";
	bUseNodeActivationNotify = true;

	OnNextChild.BindUObject(this, &UBTComposite_UtilitySelector::GetNextChildHandler);
}

FString UBTComposite_UtilitySelector::GetStaticDescription() const
{
	return TEXT("TODO: Description of node");
}

#if WITH_EDITOR

FName UBTComposite_UtilitySelector::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Sequence.Icon");
}

#endif

uint16 UBTComposite_UtilitySelector::GetInstanceMemorySize() const
{
	return sizeof(FBTUtilitySelectorMemory);
}

void UBTComposite_UtilitySelector::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemoryPtr, EBTMemoryInit::Type InitType) const
{
	FBTUtilitySelectorMemory* memory = reinterpret_cast<FBTUtilitySelectorMemory*>(NodeMemoryPtr);
	if (InitType == EBTMemoryInit::Initialize)
	{
		// Invoke the constructor for our TArray
		new (&memory->ChildExecutionOrder) FUtilitySelectorChildExecOrder();
	}
}

void UBTComposite_UtilitySelector::NotifyNodeActivation(FBehaviorTreeSearchData& SearchData) const
{
	struct FIndexedValue
	{
		int32 ChildIdx;
		int32 UtilityValue;

		inline FIndexedValue(int32 index, int32 value)
			: ChildIdx(index), UtilityValue(value)
		{
		}

		inline bool operator<(FIndexedValue const& Other) const
		{
			return this->UtilityValue > Other.UtilityValue;
		}
	};

	TArray<FIndexedValue> utilityScores;
	for (int32 iChild = 0; iChild < this->GetChildrenNum(); ++iChild)
	{
		FBTCompositeChild const& childInfo = this->Children[iChild];

		UBTComposite_UtilityNode* utilityNode = nullptr;
		if (childInfo.ChildComposite != nullptr)
		{
			utilityNode = Cast<UBTComposite_UtilityNode>(childInfo.ChildComposite);
		}

		if (utilityNode != nullptr)
		{
			utilityScores.Add(FIndexedValue(iChild, utilityNode->EvaluateUtility(SearchData.OwnerComp)));
		}
		else
		{
			// TODO: Tell user that there is a child node that is not the correct type
		}
	}

	utilityScores.Sort();

	FBTUtilitySelectorMemory* memory = GetNodeMemory<FBTUtilitySelectorMemory>(SearchData);
	check(memory);

	// The next node to execute will be the "first" child (highest utility), or the selector's parent if there are no valid children
	memory->ChildExecutionOrder.SetNumUninitialized(utilityScores.Num());
	for (int i = 0; i < utilityScores.Num(); ++i)
	{
		memory->ChildExecutionOrder[i] = utilityScores[i].ChildIdx;
	}
}

int32 UBTComposite_UtilitySelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{	
	// Potentially useful flags:
	// PrevChild == BTSpecialChild::NotInitialized // first activation
	// LastResult == EBTNodeResult::Failed

	FBTUtilitySelectorMemory* memory = GetNodeMemory<FBTUtilitySelectorMemory>(SearchData);
	check(memory);

	if (PrevChild == BTSpecialChild::NotInitialized && memory->ChildExecutionOrder.Num() > 0)
	{
		return memory->ChildExecutionOrder[0];
	}

	// If the child ran and succeeded, then this node is complete and the AI should return to the utility selector's parent
	if (LastResult == EBTNodeResult::Succeeded)
	{
		return BTSpecialChild::ReturnToParent;
	}
	else if (LastResult == EBTNodeResult::Failed || LastResult == EBTNodeResult::Aborted) // last run failed
	{
		int32 execIndexOfNextChild = memory->ChildExecutionOrder.IndexOfByKey(PrevChild) + 1;
		if (execIndexOfNextChild < memory->ChildExecutionOrder.Num())
			return memory->ChildExecutionOrder[execIndexOfNextChild];
		else
			return BTSpecialChild::ReturnToParent;
	}
	else if (memory->ChildExecutionOrder.Num() > 0)
	{
		return memory->ChildExecutionOrder[0];
	}
	else
	{
		return BTSpecialChild::ReturnToParent;
	}
}


