// Fill out your copyright notice in the Description page of Project Settings.


#include "Portfolio_AIMonster.h"
#include <Global/Portfolio_GameInstance.h>
#include <Global/Data/MonsterData.h>
#include <Global/Portfolio_Tile.h>
#include <UI/Portfolio_MiniMap_Component.h>
#include "BehaviorTree/BlackboardComponent.h"

APortfolio_AIMonster::APortfolio_AIMonster()
{
	MiniMap = CreateDefaultSubobject<UPortfolio_MiniMap_Component>(TEXT("MiniMapComponent"));
	MiniMap->SetupAttachment(RootComponent);
	MiniMap->MiniMapInit(this);
}

void APortfolio_AIMonster::BeginPlay()
{
	UPortfolio_GameInstance * Inst = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();

	if (nullptr != Inst)
	{

		CurMonsterData = Inst->GetMonsterData(DataName);

		SetAllAnimation(CurMonsterData->MapAnimation);
		SetAniState(AIState::DEATH);

	}

	Super::BeginPlay();

	if (nullptr == GetBlackboardComponent())
	{
		return;
	}

	GetBlackboardComponent()->SetValueAsEnum(TEXT("AIState"), static_cast<uint8>(AIState::IDLE));
	GetBlackboardComponent()->SetValueAsString(TEXT("TargetTag"), TEXT("Player"));
	GetBlackboardComponent()->SetValueAsFloat(TEXT("SearchRange"), CurMonsterData->SearchRange);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), CurMonsterData->AttackRange);
	FVector _Pos = GetActorLocation();
	GetBlackboardComponent()->SetValueAsVector(TEXT("OriginPos"), _Pos);

}

void APortfolio_AIMonster::Destroyed()
{
	Super::Destroyed();

	UPortfolio_GameInstance* Inst = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();

	if (nullptr == Inst)
	{
		return;
	}
	TSubclassOf<UObject> Item = Inst->GetSubClass(TEXT("Item"));

	for (size_t i = 0; i < 1; i++)
	{
		FVector RandomPos;

		RandomPos.X = UPortfolio_Global::MainRandom.FRandRange(-200, 200);
		RandomPos.Y = UPortfolio_Global::MainRandom.FRandRange(-200, 200);

		// 아이템을 드롭하도록 만들어보자.
		{
			/*
			    // 몬스터 입장에서는 그냥 아이템인지도 알필요가 없고
			    // 그냥 내가 죽을때 어떤 액터를 만들뿐이다.
			    AActor* Actor = GetWorld()->SpawnActor<AActor>(Item);
			    Actor->Tags.Add(TEXT("Item"));
			    Actor->SetActorLocation(GetActorLocation() + RandomPos);
			*/
		}
	}

}
