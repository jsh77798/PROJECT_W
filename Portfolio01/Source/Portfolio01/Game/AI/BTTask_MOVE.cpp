// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/BTTask_MOVE.h"
#include <Global/Portfolio_GlobalCharacter.h>
#include <Game/AI/Portfolio_AIController.h>
#include <Game/AI/Portfolio_MonsterEnums.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

// 일단 써치 범위 바깥으로 이동하면
// Return 상태로 전환하는 것은 2가지 상황이 존재한다.
// 1. 그냥 플레이어가 너무 멀어져서 나는 돌아간다.
// 2. 내 원래자리(만들어진 순간에 자신의 위치.)에서 너무 멀리 떨어졌다.


UBTTask_MOVE::UBTTask_MOVE()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MOVE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::MOVE);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = 100.0f;
	}


	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();

	UNavigationPath* PathPoint = PathFindNavPath(OwnerComp, TargetPos);
	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("NavPath"), PathPoint);
	GetBlackboardComponent(OwnerComp)->SetValueAsVector(TEXT("LastPos"), TargetPos);

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_MOVE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (nullptr == TargetActor)
	{
		GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
		SetStateChange(OwnerComp, AIState::RETURN);
		return;
	}

	bool Target = GetBlackboardComponent(OwnerComp)->GetValueAsBool(TEXT("Death"));

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

	//네비메쉬
	// 너무 잦은 횟수로 길을 찾는 경우를 막는 경우는 다음과 같은 방법이 있다.
	// 길을 너무 많이 찾으면 나중에 

	FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector LastPos = TargetActor->GetActorLocation();
	FVector CheckLastPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("LastPos"));

	if (LastPos != CheckLastPos)
	{
		// 이때만 갱신을 합니다.
		UNavigationPath* NewPath = PathFindNavPath(OwnerComp, LastPos);
		GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("NavPath"), NewPath);
	}

	// 이미 좋재하기 때문에 그냥 이미 존재하는 것에서 찾아오면 된다.
	UObject* NavObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("NavPath"));
	UNavigationPath* NavPath = Cast<UNavigationPath>(NavObject);

	if (nullptr == NavPath)
	{
		GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
		SetStateChange(OwnerComp, AIState::RETURN);
		return;
	}

	// 길을 제대로 못찾았다는 이야기가 된다.
	if (nullptr != NavPath && true == NavPath->PathPoints.IsEmpty())
	{
		GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
		SetStateChange(OwnerComp, AIState::RETURN);
		return;
	}

	if (nullptr != NavPath)
	{
		TargetPos = NavPath->PathPoints[1];
	}


	TargetPos.Z = 0.0f;
	ThisPos.Z = 0.0f;
	LastPos.Z = 0.0f;

	{
		FVector Dir = TargetPos - ThisPos;
		Dir.Normalize();

		FVector OtherForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		OtherForward.Normalize();

		FVector Cross = FVector::CrossProduct(OtherForward, Dir);

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		if (FMath::Abs(Angle0 - Angle1) >= 10.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DelataSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		else 
		{
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}



	{
		FVector Dir = TargetPos - ThisPos;

		GetGlobalCharacter(OwnerComp)->AddMovementInput(Dir);
		//GetGlobalCharacter(OwnerComp)->SetActorRotation(Dir.Rotation());

		FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));
		float SearchRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("SearchRange"));
		float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));

		OriginPos.Z = 0.0f;
		FVector OriginDir = OriginPos - ThisPos;
		
		if (OriginDir.Size() >= SearchRange * 1.5f)
		{
			//GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
			SetStateChange(OwnerComp, AIState::RETURN);
			int a = 0;
			return;
		}
		

		FVector LastDir = LastPos - ThisPos;

		
		if (SearchRange < LastDir.Size())
		{
			//GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), nullptr);
			SetStateChange(OwnerComp, AIState::RETURN);
			return;
		}
		

		// 충분히 근접했다.
		if (AttackRange >= LastDir.Size())
		{
			SetStateChange(OwnerComp, AIState::ATTACK);
			return;
		}
	}
}
