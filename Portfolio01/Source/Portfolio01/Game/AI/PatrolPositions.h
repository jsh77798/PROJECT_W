// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PatrolPositions.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UPatrolPositions : public UObject
{
	GENERATED_BODY()

public:
	int CurrentIndex = 0;
	TArray<FVector> Positions;
};
