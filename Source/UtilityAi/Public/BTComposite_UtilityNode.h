// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "BTComposite_UtilityNode.generated.h"

class UCurveFloat;

USTRUCT(BlueprintType)
struct FUtilityInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = UtilityAI)
		FBlackboardKeySelector Key;

	UPROPERTY(EditAnywhere, Category = UtilityAI)
		UCurveFloat* Curve;

};

/**
 * 
 */
UCLASS()
class UTILITYAI_API UBTComposite_UtilityNode : public UBTCompositeNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = UtilityAI)
		TArray<FUtilityInput> Inputs;

	UPROPERTY(EditAnywhere, Category = UtilityAI)
		bool CanExecuteUtilityLTEZero;
	
public:
	UBTComposite_UtilityNode(const FObjectInitializer& ObjectInitializer);
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;
		
	float EvaluateUtility(UBehaviorTreeComponent const &OwnerComp) const;

	bool const CanExecute(float utilityValue) const;

};
