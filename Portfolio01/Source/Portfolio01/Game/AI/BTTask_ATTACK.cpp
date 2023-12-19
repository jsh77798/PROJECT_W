// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/BTTask_ATTACK.h"


UBTTask_ATTACK::UBTTask_ATTACK()
{
	// 매 프레임마다 실행해라
	bNotifyTick = true;
	// 끝나는 순간 한번 실행해라.
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_ATTACK::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FRandomStream RandomStream(FMath::Rand());
	// 1부터 5까지의 무작위 정수 생성
	int32 RandomNumber = RandomStream.RandRange(1, 3);
	if (RandomNumber == 1)
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::ATTACK);
	}
	if (RandomNumber == 2)
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::ATTACK2);
	}
	if (RandomNumber == 3)
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::ATTACK3);
	}
	//GetGlobalCharacter(OwnerComp)->SetAniState(UBTTask_AIBase::GetAiState(OwnerComp));

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_ATTACK::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);
	

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

	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(UBTTask_AIBase::GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		//GetGlobalCharacter(OwnerComp)->SetAttackAnimcheck(false);
		SetStateChange(OwnerComp, AIState::MOVE);
	}
}

