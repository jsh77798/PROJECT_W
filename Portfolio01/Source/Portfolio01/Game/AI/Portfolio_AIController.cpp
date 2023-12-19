// Fill out your copyright notice in the Description page of Project Settings.


#include "Portfolio_AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AICharacter.h"

APortfolio_AIController::APortfolio_AIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));;
}

// 빙의 순간 
// 캐릭터만들어지고 이제부터 컨트롤러가 이캐릭터를 조작할것이다.
void APortfolio_AIController::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);

	if (nullptr != BehaviorTreeComponent && true == BehaviorTreeComponent->IsValidLowLevel())
	{
		AAICharacter* AIPawn = Cast<AAICharacter>(_InPawn);

		UBehaviorTree* BehaviorTree = AIPawn->GetBehaviorTree();
		//BehaviorTreeComponent->StartTree(*BehaviorTree);

		if (nullptr == BehaviorTree || false == BehaviorTree->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == BehaviorTree || false == BehaviorTree->IsValidLowLevel())"), __FUNCTION__, __LINE__);
			return;
		}
	    BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

	    BlackboardComponent->SetValueAsObject(TEXT("SelfActor"), _InPawn);

	    BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
	
}

