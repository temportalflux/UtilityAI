// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BTComposite_UtilityNode.generated.h"

/**
 * 
 */
UCLASS()
class UTILITYAI_API UBTComposite_UtilityNode : public UBTCompositeNode
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		int32 UtilityValue;
	
public:
	UBTComposite_UtilityNode(const FObjectInitializer& ObjectInitializer);
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;
		
	int32 EvaluateUtility() const;

};
