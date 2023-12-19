// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portfolio_Widget_Main.h"
#include "UI/Portfolio_Widget_Inventory.h"
#include "Game/Portfolio_Character.h"
#include <Global/Data/PlayerData.h>
#include "Global/Portfolio_GlobalCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Game/Portfolio_Enums.h"

void UPortfolio_Widget_Main::NativeConstruct()
{
	Super::NativeConstruct();

	// Inven = Cast<UInventoryUserWidget>(GetWidgetFromName(TEXT("WBP_UI_Inventory")));
	// Inven = Cast<UInventoryUserWidget>(GetWidgetFromName(TEXT("WBP_Status")));


	AllWidGet.Add(Cast<UUserWidget>(GetWidgetFromName(TEXT("WBP_UI_Inventory"))));
	AllWidGet.Add(Cast<UUserWidget>(GetWidgetFromName(TEXT("WBP_UI_MiniMap"))));
	AllWidGet.Add(Cast<UUserWidget>(GetWidgetFromName(TEXT("WBP_UI_Option"))));
	AllWidGet.Add(Cast<UUserWidget>(GetWidgetFromName(TEXT("WBP_UI_BackGround"))));

}

void UPortfolio_Widget_Main::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	// 기본적인 이동이나 이런 시스템자체가 다르다는것을 기억해야 한다.
	// Inven->SetPositionInViewport({100, 100});
	// Inven->SetAnchorsInViewport();
}

// 여기서 리턴하는 bool 값은 지금 상태가 바뀌었다는 AllHidden이 바뀌었다는것을 체크하는 bool 값이 된다.
bool UPortfolio_Widget_Main::CheckAllWidGetHidden()
{
	bool AllHiddenCheck = true;

	for (size_t i = 0; i < AllWidGet.Num(); i++)
	{
		if (AllWidGet[i]->GetVisibility() == ESlateVisibility::Visible)
		{
			AllHiddenCheck = false;
		}
	}

	// false였는데 true가 되고
	// true였는데 false가 되는 상황이 오면
	if (AllHiddenCheck != AllHidden)
	{
		AllHidden = AllHiddenCheck;
		return true;
	}

	return false;
}

bool UPortfolio_Widget_Main::CheckAiming()
{
	// Portfolio_Character에서 Aiming상태를 체크하고 값을 가져온다
    APortfolio_Character* MyCharacter = Cast<APortfolio_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));  //UGameplayStatics => #include "Kismet/GameplayStatics.h"헤더 필요
	bool AimingCheck = MyCharacter->AimingCheck;

	return AimingCheck;
}

bool UPortfolio_Widget_Main::CheckRangeItem()
{
	
	return true;
}

int UPortfolio_Widget_Main::SetMyCharacter()
{
	APortfolio_Character* MyCharacter = Cast<APortfolio_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	int _Bullet = MyCharacter->Bullet;
	return _Bullet;
}


int UPortfolio_Widget_Main::SetMyCharacter2()
{
	APortfolio_Character* MyCharacter = Cast<APortfolio_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	int _LoadBullet = MyCharacter->LoadBullet;
	return _LoadBullet;
}
