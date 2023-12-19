// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Portfolio_MiniMap_Component.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Global/Portfolio_GlobalCharacter.h>

// Sets default values for this component's properties
UPortfolio_MiniMap_Component::UPortfolio_MiniMap_Component()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPortfolio_MiniMap_Component::MiniMapInit(APortfolio_GlobalCharacter* _Owner)
{
	SpringArmComponent = _Owner->CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->TargetArmLength = 5000.0f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetupAttachment(this);
	SpringArmComponent->bUsePawnControlRotation = false;;
	SpringArmComponent->bInheritPitch = false;
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritRoll = false;


	_Owner->PushComponent(SpringArmComponent);

	FVector Dir = FVector(0.0f, 0.0f, -1.0f);
	SpringArmComponent->SetRelativeRotation(Dir.Rotation());
	FString Path = TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'");
	ConstructorHelpers::FObjectFinder<UStaticMesh> DataTable(*Path);
	UStaticMesh* StaticMesh;


	if (DataTable.Succeeded())
	{
		StaticMesh = DataTable.Object;
		StaticMeshComponent = _Owner->CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetupAttachment(SpringArmComponent);
		_Owner->PushComponent(StaticMeshComponent);
		// StaticMeshComponent->roation
		// FVector Dir = FVector(0.0f, 0.0f, 1.0f);
		// StaticMeshComponent->AddLocalRotation();
	}
}

// Called when the game starts
void UPortfolio_MiniMap_Component::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPortfolio_MiniMap_Component::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

