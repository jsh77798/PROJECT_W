// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Portfolio_AIController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API APortfolio_AIController : public AAIController
{
	GENERATED_BODY()

public:
	APortfolio_AIController();

protected:
	void OnPossess(APawn* _InPawn) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBlackboardComponent* BlackboardComponent;
};
