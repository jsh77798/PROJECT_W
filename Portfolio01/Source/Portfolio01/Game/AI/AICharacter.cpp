// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Portfolio_AIController.h"

UBlackboardComponent* AAICharacter::GetBlackboardComponent()
{
	if (nullptr == BlackboardComponent)
	{
		APortfolio_AIController* AiCon = GetController<APortfolio_AIController>();

		if (nullptr == AiCon)
		{
			return nullptr;
		}

		BlackboardComponent = AiCon->GetBlackboardComponent();
	}

	return BlackboardComponent;
}