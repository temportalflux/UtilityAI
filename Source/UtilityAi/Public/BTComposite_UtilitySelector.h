// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BTComposite_UtilitySelector.generated.h"

#define MAX_UTILITY_CHILDREN 16
typedef TArray< uint8, TFixedAllocator< MAX_UTILITY_CHILDREN > > FUtilitySelectorChildExecOrder;

struct FBTUtilitySelectorMemory : public FBTCompositeMemory
{
	/** ordering of child nodes for the current search activation (array is a list of child indexes arranged in execution order) */
	FUtilitySelectorChildExecOrder ChildExecutionOrder;
};

/**
 * 
 */
UCLASS()
class UTILITYAI_API UBTComposite_UtilitySelector : public UBTCompositeNode
{
	GENERATED_BODY()
	
public:
	UBTComposite_UtilitySelector(const FObjectInitializer& ObjectInitializer);
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual uint16 GetInstanceMemorySize() const override;
	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const;

protected:
	/** called when start enters this node */
	virtual void NotifyNodeActivation(FBehaviorTreeSearchData& SearchData) const override;
	
};
