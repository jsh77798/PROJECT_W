// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/BTTask_IDLE.h"
#include <Global/Portfolio_GlobalCharacter.h>
#include <Game/AI/Portfolio_AIController.h>
#include <Game/AI/Portfolio_MonsterEnums.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"

EBTNodeResult::Type UBTTask_IDLE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FRandomStream RandomStream(FMath::Rand());
	// 1부터 5까지의 무작위 정수 생성
	int32 RandomNumber = RandomStream.RandRange(1, 3);
	if (RandomNumber == 1)
	{
	    GetGlobalCharacter(OwnerComp)->SetAniState(AIState::IDLE);
	}
	if (RandomNumber == 2)
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::IDLE2);
	}
	if (RandomNumber == 3)
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::IDLE3);
	}

	//GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_IDLE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	if (true == IsDeathCheck(OwnerComp))
	{
		SetStateChange(OwnerComp, AIState::DEATH);
		return;
	}

	if (true == IsStunCheck(OwnerComp))
	{
		SetStateChange(OwnerComp, AIState::STUN);
		return;
	}

	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);


	if (2.0f <= GetStateTime(OwnerComp))
	{
		// 지역변수로 랜덤을 이용하는것.
		FRandomStream Stream;
		// 알아서 시간과 같은 변하는 수를 시드값으로 삼는다.
		Stream.GenerateNewSeed();

		int Value = Stream.RandRange(0, 1);

		// UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, Value);

		ResetStateTime(OwnerComp);

		//if (Value == 0)
		//{
		SetStateChange(OwnerComp, AIState::PATROL);
		return;
		//}

		// 그럼 순찰할까?
		// 그냥 5초간 다시 대기하자.
		// SetStateChange(OwnerComp, AIState::MOVE);
		// return;
	}

	AActor* ResultActor = GetTargetSearch(OwnerComp);

	if (nullptr != ResultActor)
	{
		GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), ResultActor);
		SetStateChange(OwnerComp, AIState::MOVE);
		return;
	}

	//핵심은 플레이어를 찾아내는것.
	// GetGlobalCharacter(OwnerComp)->GetLevel()->;
	return;
}