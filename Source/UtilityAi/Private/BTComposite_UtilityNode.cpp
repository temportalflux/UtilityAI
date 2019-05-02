// Fill out your copyright notice in the Description page of Project Settings.

#include "BTComposite_UtilityNode.h"
#include "Math.h"
#include "UtilityAi.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Curves/CurveFloat.h"

UBTComposite_UtilityNode::UBTComposite_UtilityNode(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Utility Node";
	bUseNodeActivationNotify = true;

	//OnNextChild.BindUObject(this, &UBTComposite_UtilityNode::GetNextChildHandler);
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

float const resolveBlackboardKeyValue(FBlackboardKeySelector key, UBlackboardComponent const *pBlackboard)
{
	key.ResolveSelectedKey(*pBlackboard->GetBlackboardAsset());
	// Bool => 0 or 1
	if (key.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass()) return pBlackboard->GetValue<UBlackboardKeyType_Bool>(key.GetSelectedKeyID()) ? 1.0f : 0.0f;
	// Enum integer value as float
	else if (key.SelectedKeyType == UBlackboardKeyType_Enum::StaticClass()) return (float)pBlackboard->GetValue<UBlackboardKeyType_Enum>(key.GetSelectedKeyID());
	else if (key.SelectedKeyType == UBlackboardKeyType_NativeEnum::StaticClass()) return (float)pBlackboard->GetValue<UBlackboardKeyType_NativeEnum>(key.GetSelectedKeyID());
	// Raw float
	else if (key.SelectedKeyType == UBlackboardKeyType_Float::StaticClass()) return pBlackboard->GetValue<UBlackboardKeyType_Float>(key.GetSelectedKeyID());
	// Int to float
	else if (key.SelectedKeyType == UBlackboardKeyType_Int::StaticClass()) return (float)pBlackboard->GetValue<UBlackboardKeyType_Int>(key.GetSelectedKeyID());
	// Magnitude of a vector (sqrt involved)
	else if (key.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass()) return pBlackboard->GetValue<UBlackboardKeyType_Vector>(key.GetSelectedKeyID()).Size();
	// Undefinied
	else if (key.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass()) return 0;
	else if (key.SelectedKeyType == UBlackboardKeyType_Name::StaticClass()) return 0;
	else if (key.SelectedKeyType == UBlackboardKeyType_String::StaticClass()) return 0;
	else if (key.SelectedKeyType == UBlackboardKeyType_Class::StaticClass()) return 0;
	else if (key.SelectedKeyType == UBlackboardKeyType_Object::StaticClass()) return 0;
	else
	{
		UE_LOG(LogUtilityAi, Log, TEXT("No class type found for blackboard key %s"), *key.SelectedKeyName.ToString());
		return 0;
	}
}

float UBTComposite_UtilityNode::EvaluateUtility(UBehaviorTreeComponent const &OwnerComp) const
{
	//return FMath::Rand() % this->UtilityValue;
	//return this->UtilityValue;
	float inputUtilitySum = 0;
	
	UBlackboardComponent const * blackboard = OwnerComp.GetBlackboardComponent();

	for (auto input : this->Inputs)
	{
		if (input.Curve == nullptr) continue;
		float blackboardValue = resolveBlackboardKeyValue(input.Key, blackboard);
		inputUtilitySum += input.Curve->GetFloatValue(blackboardValue);
	}

	return inputUtilitySum;
}

bool const UBTComposite_UtilityNode::CanExecute(float utilityValue) const
{
	return utilityValue > 0 || this->CanExecuteUtilityLTEZero;
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
