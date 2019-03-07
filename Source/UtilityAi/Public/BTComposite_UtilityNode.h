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

	UPROPERTY(EditAnywhere)
		FBlackboardKeySelector Key;
	UPROPERTY(EditAnywhere)
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

	UPROPERTY(EditAnywhere)
		TArray<FUtilityInput> Inputs;
	
public:
	UBTComposite_UtilityNode(const FObjectInitializer& ObjectInitializer);
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;
		
	int32 EvaluateUtility(UBehaviorTreeComponent const &OwnerComp) const;

};
