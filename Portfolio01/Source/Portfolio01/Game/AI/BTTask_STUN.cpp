// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/BTTask_STUN.h"
#include <Global/Portfolio_GlobalCharacter.h>
#include <Game/AI/Portfolio_AIController.h>
#include <Game/AI/Portfolio_MonsterEnums.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "Kismet/GameplayStatics.h"
#include "Math/NumericLimits.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTask_STUN::UBTTask_STUN()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_STUN::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	FName BoneNameStun = GetGlobalCharacter(OwnerComp)->GetBoneName();
	if (BoneNameStun == "head")
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::STUN); // Çìµå¼¦
	}

	if (BoneNameStun == "pelvis" || BoneNameStun == "spine_01" || BoneNameStun == "spine_02")
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::ABDOMEN_STUN); // ¸öÅë¼¦
	}

	if (BoneNameStun == "upperarm_l" || BoneNameStun == "lowerarm_l" || BoneNameStun == "hand_l")
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::LEFT_ARM_STUN); // ¿ÞÆÈ
	}

	if (BoneNameStun == "upperarm_r" || BoneNameStun == "lowerarm_r" || BoneNameStun == "hand_r")
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::RIGHT_ARM_STUN); // ¿À¸¥ÆÈ
	}

	if (BoneNameStun == "thigh_l" || BoneNameStun == "calf_l" || BoneNameStun == "foot_l")
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::LEFT_LEG_STUN); // ¿ÞÂÊ ´Ù¸®
	}

	if (BoneNameStun == "thigh_r" || BoneNameStun == "calf_r" || BoneNameStun == "foot_r")
	{
		GetGlobalCharacter(OwnerComp)->SetAniState(AIState::RIGHT_LEG_STUN); // ¿À¸¥ÂÊ ´Ù¸®
	}

	//ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_STUN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
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

	/*
	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(UBTTask_AIBase::GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	AActor* ResultActor = GetTargetSearch(OwnerComp);

	if (Time <= GetStateTime(OwnerComp))
	{
		if (nullptr != ResultActor)
		{
			GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), ResultActor);
			SetStateChange(OwnerComp, AIState::MOVE);
			return;
		}
		//GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
		SetStateChange(OwnerComp, AIState::RETURN);
	}
	*/

	UAnimMontage* Montage = GetGlobalCharacter(OwnerComp)->GetAnimMontage(UBTTask_AIBase::GetAiState(OwnerComp));
	float Time = Montage->CalculateSequenceLength();

	if (Time <= GetStateTime(OwnerComp))
	{
		SetStateChange(OwnerComp, AIState::MOVE);
	}
}
