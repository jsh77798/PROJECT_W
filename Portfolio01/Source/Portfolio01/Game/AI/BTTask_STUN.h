// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_AIBase.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_STUN.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UBTTask_STUN : public UBTTask_AIBase
{
	GENERATED_BODY()

	UBTTask_STUN();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds);
	//FVector ThisPos;

};
