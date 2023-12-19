// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portfolio_HUD.h"


APortfolio_HUD::APortfolio_HUD()
{

}

APortfolio_HUD::~APortfolio_HUD()
{

}

void APortfolio_HUD::BeginPlay()
{
	Super::BeginPlay();

	// 존재하는(WBP_UI_Main) 블루프린트의 레퍼런스 경로를 가져온다.
	FSoftClassPath ClassPath(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BluePrint/UI/WBP_GameUI/WBP_UI_Main.WBP_UI_Main_C'"));
	TSubclassOf<UUserWidget> MainWidgetClass = ClassPath.TryLoadClass<UUserWidget>();
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), MainWidgetClass);
	MainWidget = Cast<UPortfolio_Widget_Main>(Widget);
	MainWidget->AddToViewport();

	
	{
		UUserWidget* Window = Cast<UUserWidget>(MainWidget->GetWidgetFromName(TEXT("MapWindow")));
	}

	{
		UUserWidget* Window = Cast<UUserWidget>(MainWidget->GetWidgetFromName(TEXT("OptionWindow")));
	}
	
}

void APortfolio_HUD::Tick(float _Delta)
{
	Super::Tick(_Delta);
}
