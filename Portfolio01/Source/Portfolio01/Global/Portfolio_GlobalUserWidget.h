// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Portfolio_GlobalUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API UPortfolio_GlobalUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void AddDebugText(const FString& _Text)
	{
		DebugTextTick += _Text + TEXT("\n");
	}

	UPROPERTY(BlueprintReadWrite)
		FString DebugTextTick;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
