// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include <Game/AI/Portfolio_MonsterEnums.h>
#include <Global/Data/MonsterData.h>
#include "Portfolio_AIMonster.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API APortfolio_AIMonster : public AAICharacter
{
	GENERATED_BODY()

public:
	APortfolio_AIMonster();

	const struct FMonsterData* CurMonsterData;

	void BeginPlay() override;

	void Destroyed() override;
	
	//void MonsterPrePos(/*FVector _PrePos*/);
	//FVector GetPrePos();
	FVector _PrePos;
	FVector MPrePos;
	int A = 0;

private:

	UPROPERTY(Category = "Animation", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName DataName = "NONE";

	UPROPERTY(Category = "Monster", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UPortfolio_MiniMap_Component* MiniMap;
	//const FMonsterData* CurMonsterData;
};
