// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include <UI/Portfolio_Widget_Main.h>
#include "Portfolio_HUD.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO01_API APortfolio_HUD : public AHUD
{
	GENERATED_BODY()

public:
	APortfolio_HUD();
	~APortfolio_HUD();

	// 함수-> UPortfolio_Widget_Main* MainWidget를 반환한다
	UPortfolio_Widget_Main* GetMainWidget()
	{
		return MainWidget;
	}

protected:
	void BeginPlay() override;
	void Tick(float _Delta) override;

private:
	UPortfolio_Widget_Main* MainWidget = nullptr;
};
