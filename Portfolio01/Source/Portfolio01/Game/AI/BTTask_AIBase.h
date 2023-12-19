// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Global/Portfolio_GlobalCharacter.h"
#include "Game/AI/Portfolio_AIController.h"
#include "Game/AI/Portfolio_MonsterEnums.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "BTTask_AIBase.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UBTTask_AIBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_AIBase();


	void OnGameplayTaskActivated(class UGameplayTask&) override;

	float GetStateTime(UBehaviorTreeComponent& OwnerComp);

	AIState GetAiState(UBehaviorTreeComponent& OwnerComp);

	void ResetStateTime(UBehaviorTreeComponent& OwnerComp);

	template<typename EnumType>
	void SetStateChange(UBehaviorTreeComponent& OwnerComp, EnumType _State)
	{
		SetStateChange(OwnerComp, static_cast<uint8>(_State));
	}

	void SetStateChange(UBehaviorTreeComponent& OwnerComp, uint8 _State);

	class APortfolio_GlobalCharacter* GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp);

	class AActor* GetTargetSearch(UBehaviorTreeComponent& OwnerComp);

	class UBlackboardComponent* GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp);


	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds) override;
	
	bool IsDeathCheck(UBehaviorTreeComponent& OwnerComp);

	bool IsAttackCheck(UBehaviorTreeComponent& OwnerComp);

	bool IsStunCheck(UBehaviorTreeComponent& OwnerComp);

	TArray<FVector> PathFind(UBehaviorTreeComponent& OwnerComp, AActor* _Actor);

	TArray<FVector> PathFind(UBehaviorTreeComponent& OwnerComp, FVector _Pos);

	UNavigationPath* PathFindNavPath(UBehaviorTreeComponent& OwnerComp, AActor* _Actor);

	UNavigationPath* PathFindNavPath(UBehaviorTreeComponent& OwnerComp, FVector _Pos);

};
