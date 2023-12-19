// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portfolio_MiniMap_Actor.h"

// Sets default values
APortfolio_MiniMap_Actor::APortfolio_MiniMap_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APortfolio_MiniMap_Actor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APortfolio_MiniMap_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (nullptr == TraceActor)
	{
		return;
	}

	SetActorLocation(TraceActor->GetActorLocation());
}

