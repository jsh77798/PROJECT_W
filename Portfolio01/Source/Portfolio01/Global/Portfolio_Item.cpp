// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Portfolio_Item.h"
#include "Global/Portfolio_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include <UI/Portfolio_Widget_Inventory.h>
#include <UI/Portfolio_HUD.h>

// Sets default values
APortfolio_Item::APortfolio_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APortfolio_Item::BeginPlay()
{
	Super::BeginPlay();
	// 
	UPortfolio_GameInstance* GameInstance = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();
	Data = GameInstance->GetRandomItemData();


	if (nullptr != Data->Mesh)
	{
		// 내부에 static매쉬 컴포넌트가 있는지 찾고
		UActorComponent* Component = GetComponentByClass(UStaticMeshComponent::StaticClass());
		UStaticMeshComponent* StaticMesh = Cast<UStaticMeshComponent>(Component);
		if (nullptr != StaticMesh)
		{
			StaticMesh->SetStaticMesh(Data->Mesh);
		}
	}
}

// Called every frame
void APortfolio_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APortfolio_Item::Take(AActor* _Actor)
{
	// 이 아이템을 먹는게 무조건 절대로 이 세상이 쪼개져도 플레이어라는게 확실하면.
	// 아이템
	// UInventoryUserWidget* WidGet;
	// dlrjf 

	// static의 장점
	// 메모리 크러쉬를 낼수 있다.

	// 가장 좋은 건 GameMode입니다.
	// 
	// UInventoryUserWidget::InvenWidget;

	// 절대로 안나온다고 확답을 받아야 한다.
	// 무조건 먹었으면 플레이어라고 생각한다.
	// 이게 무조건 또 플레이어가 아닐수 있다는 가정하에.

	// 인벤토리를 가져와서 인벤토리에 넣어버린다.

	APlayerController* Con = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	APortfolio_HUD* HUD = Con->GetHUD<APortfolio_HUD>();
	if (nullptr == HUD)
	{
		// 인벤토리가 없을 것이다.
		return;
	}

	UPortfolio_Widget_Main* WidGet = HUD->GetMainWidget();
	if (nullptr == WidGet)
	{
		return;
	}

	UWidget* InvenWidGet = WidGet->GetWidgetFromName(TEXT("WBP_UI_Inventory"));
	UPortfolio_Widget_Inventory* Inventory = Cast<UPortfolio_Widget_Inventory>(InvenWidGet);

	if (nullptr == Inventory)
	{
		return;
	}

	Inventory->AddGameItem(Data);

	// Inventory->AddInvenItem()

	int a = 0;

}