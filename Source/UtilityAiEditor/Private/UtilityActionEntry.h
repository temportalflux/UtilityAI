#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"

struct FUtilityActionEntry
{
	bool mShouldCreate;
	FName mName;
	UBlackboardKeyType* mpType;
};
