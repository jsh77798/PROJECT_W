// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Global/Portfolio_GlobalCharacter.h"
#include "AICharacter.generated.h"

UCLASS()
class PORTFOLIO01_API AAICharacter : public APortfolio_GlobalCharacter
{
	GENERATED_BODY()

public:
	
	FORCEINLINE class UBehaviorTree* GetBehaviorTree()
	{
		return BehaviorTree;
	}

	class UBlackboardComponent* GetBlackboardComponent();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
		class UBlackboardComponent* BlackboardComponent;

};
