// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_AIBase.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RETURN.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UBTTask_RETURN : public UBTTask_AIBase
{
	GENERATED_BODY()

		UBTTask_RETURN();

	//FVector R_PrePos;
	FVector PrePos;
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds);
};


