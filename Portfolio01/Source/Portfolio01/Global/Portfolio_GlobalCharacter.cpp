// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Portfolio_GlobalCharacter.h"
#include <Global/Portfolio_GameInstance.h>
#include <Global/Data/PlayerData.h>
#include <Global/Data/TileData.h>
#include <Global/Portfolio_Tile.h>
#include "Game/AI/BTTask_ATTACK.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APortfolio_GlobalCharacter::APortfolio_GlobalCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APortfolio_GlobalCharacter::BeginPlay()
{
	Super::BeginPlay();


	Portfolio_GlobalAnimInstance = Cast<UPortfolio_GlobalAnimInstance>(GetMesh()->GetAnimInstance());

	Portfolio_GlobalAnimInstance->AllAnimations = AllAnimations;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APortfolio_GlobalCharacter::BeginOverLap);
	//GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APortfolio_GlobalCharacter::EndOverLap);
}

// Called every frame
void APortfolio_GlobalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APortfolio_GlobalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void APortfolio_GlobalCharacter::BeginOverLap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	
	//Damage Tag에 접촉시 사용
	if (true == OtherComp->ComponentHasTag(TEXT("Damage")))
	{
		DamageCheck = true;
		AttackAnimCheck = true;
		if (AttackAnimCheck == true)
		{
			PlayerHP -= 90;
		}
	}

}

/*
void APortfolio_GlobalCharacter::EndOverLap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	DamageCheck = false;
}
*/
float APortfolio_GlobalCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Att += DamageAmount;
	HP -= DamageAmount;
	

	return DamageAmount;
}

bool APortfolio_GlobalCharacter::TakeDamageBone(bool _BoneCheck, FName _BoneName, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	BoneName = _BoneName;
	BoneCheck = _BoneCheck;
	return _BoneCheck;
}



