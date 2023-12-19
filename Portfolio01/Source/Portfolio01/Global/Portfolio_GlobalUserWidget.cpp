// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Portfolio_GlobalUserWidget.h"

void UPortfolio_GlobalUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	DebugTextTick = "";
}