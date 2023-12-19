// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Portfolio_Tile.h"
#include <Global/Portfolio_GameInstance.h>
#include <Global/Data/PlayerData.h>
#include <Global/Data/TileData.h>
#include <Global/Portfolio_GlobalCharacter.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"


// Sets default values
APortfolio_Tile::APortfolio_Tile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"), true);
	SphereComponent->ComponentTags.Add(FName("Damage"));
	//SphereComponent->OnComponentHit.AddDynamic(this, &APortfolio_Tile::OnHit);


	
	//UStaticMeshComponent* ProjectileMovement = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMovement"));
	//ProjectileMovement->UpdatedComponent = SphereComponent;
	/*
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 1000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	*/
}


// Called when the game starts or when spawned
void APortfolio_Tile::BeginPlay()
{
	Super::BeginPlay();

	//PlayerAtt = CurPlayerData->ATT;

	GetLocation = GetActorLocation();

	OnDestroyed.AddDynamic(this, &APortfolio_Tile::DestroyProjectile);
	// SphereComponent->SetCollisionProfileName(TEXT("MonsterAttack"), true);
}

void APortfolio_Tile::DestroyProjectile(AActor* _Destroy)
{
	if (nullptr == DeathCreateObject)
	{
		return;
	}

	AActor* Actor = GetWorld()->SpawnActor<AActor>(DeathCreateObject);

	Actor->SetActorLocation(GetActorLocation());
	Actor->SetActorRotation(GetActorRotation());
}

// Called every frame
void APortfolio_Tile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DeathTime -= DeltaTime;

		if (DeathTime < 0.0f)
		{
			Destroy();
			return;
		}

		
        AddActorWorldOffset(GetActorForwardVector()  * DeltaTime * Speed);
		
		
		//if (DeltaTime >= 0.01f)
		//{

		//PlayerAtt에 값을 저장하여, 적중시 HP계산할때 사용한다

		//GetData(PlayerAtt);

		    //UPortfolio_GameInstance* Inst = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();
			//Inst->GetGameData();
		//}
}

/*
void APortfolio_Tile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		// Calculate the distance between the bullet and the hit location.
		
		float Distance = (Hit.ImpactPoint - GetActorLocation()).Size();
		//float Impact = (Hit.ImpactPoint).Size();
		// Calculate damage based on distance.
		float Damage = FMath::Lerp(1.0f, MaxDamageDistance, Distance) * DamagePerUnit;

		// Apply damage to the hit actor (assuming it's an ACharacter).
		APortfolio_GlobalCharacter* HitCharacter = Cast<APortfolio_GlobalCharacter>(OtherActor);
		if (HitCharacter)
		{
			HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
			int a = 0;
		}

		// Destroy the bullet.
		Destroy();
	}
}
*/

void APortfolio_Tile::AimingAttack(FVector CameraLoc, FVector CameraForward)
{
	
	FVector StartLoc = CameraLoc; // 레이저 시작 지점.
	FVector EndLoc = CameraLoc + (CameraForward * 5000.0f); // 레이저 끝나는 지점. //중심부 탄환
	FVector EndLoc_L = EndLoc + FVector(-150.0f, 0.0f, 0.0f) ; //좌측부 탄환
	FVector EndLoc_R = EndLoc + FVector(150.0f, 0.0f, 0.0f); //우측부 탄환
	FVector EndLoc_F = EndLoc + FVector(0.0f, 0.0f, -150.0f); //상단부 탄환
	FVector EndLoc_B = EndLoc + FVector(0.0f, 0.0f, 150.0f); //하단부 탄환

	/*
	FVector Direction = (EndLoc - SphereComponent->GetComponentLocation()).GetSafeNormal();
	FVector Movement = Direction * 3000 * GetWorld()->GetDeltaSeconds();
	SphereComponent->AddWorldOffset(Movement);
	*/
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // 히트 가능한 오브젝트 유형들.
	TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
	TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
	ECollisionChannel CollisionChannel;
	FCollisionResponseParams ResponseParams;
	if (UCollisionProfile::GetChannelAndResponseParams(FName(TEXT("Monster")), CollisionChannel, ResponseParams))
	{
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(CollisionChannel));
	}
	ObjectTypes.Add(WorldStatic);
	ObjectTypes.Add(WorldDynamic);

	TArray<AActor*> IgnoreActors; // 무시할 액터들.
	
	FHitResult HitResult; // 히트 결과 값 받을 변수.
	FHitResult HitResult_L;
	FHitResult HitResult_R;
	FHitResult HitResult_F;
	FHitResult HitResult_B;

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors, 
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		
	);

	// 좌측 라인트레이스
	bool Result_L = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc_L,
		ObjectTypes,
		false,
		IgnoreActors, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::ForDuration,
		HitResult_L,
		true
	);

	// 우측 라인트레이스
	bool Result_R = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc_R,
		ObjectTypes,
		false,
		IgnoreActors, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::ForDuration,
		HitResult_R,
		true
	);

	// 상단측 라인트레이스
	bool Result_F = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc_F,
		ObjectTypes,
		false,
		IgnoreActors, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::ForDuration,
		HitResult_F,
		true
	);

	// 하단측 라인트레이스
	bool Result_B = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc_B,
		ObjectTypes,
		false,
		IgnoreActors, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::ForDuration,
		HitResult_B,
		true
	);

	if (Result_L == true || Result_R == true || Result_F == true || Result_B == true)
	{
		UPrimitiveComponent* HitComponent_L = HitResult_L.GetComponent();
		UPrimitiveComponent* HitComponent_R = HitResult_R.GetComponent();
		UPrimitiveComponent* HitComponent_F = HitResult_F.GetComponent();
		UPrimitiveComponent* HitComponent_B = HitResult_B.GetComponent();

		if (HitComponent_L) 
		{
			OtherDamage(HitResult_L);
		}

		if (HitComponent_R)
		{
			OtherDamage(HitResult_R);
		}

		if (HitComponent_F)
		{
			OtherDamage(HitResult_F);
		}

		if (HitComponent_B)
		{
			OtherDamage(HitResult_B);
		}	
	}

	if (Result == true)
	{
		float Damage = 100;
		FVector ImpactPoint = HitResult.ImpactPoint;
		HitResult.GetActor();
		// HitResult에서 필요한 값들을 사용하면 됨.

		//float Distance = (HitResult.ImpactPoint - GetActorLocation()).Size();
		//float Impact = (Hit.ImpactPoint).Size();
		// Calculate damage based on distance.
		//float Damage = FMath::Lerp(1.0f, MaxDamageDistance, Distance) * DamagePerUnit;

		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent)
		{
			FName BoneName = HitResult.BoneName; // 바디 콜리전의 본 이름을 가져옵니다.
			// 이제 BoneName을 사용하여 바디 콜리전과 관련된 작업을 수행할 수 있습니다.

			// Apply damage to the hit actor (assuming it's an ACharacter).
			APortfolio_GlobalCharacter* HitCharacter = Cast<APortfolio_GlobalCharacter>(HitResult.GetActor());

			if (BoneName == "Head") // 헤드샷( +75% )
			{
				if (HitCharacter)
				{
					float ExtraDamage = Damage * 0.75;
					Damage += ExtraDamage;
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "pelvis" || BoneName == "spine_01" || BoneName == "spine_02") // 몸통샷( +20% )
			{
				if (HitCharacter)
				{
					float ExtraDamage = Damage * 0.20;
					Damage += ExtraDamage;
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "upperarm_l" || BoneName == "lowerarm_l" || BoneName == "hand_l") // 왼팔( )
			{
				if (HitCharacter)
				{
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "upperarm_r" || BoneName == "lowerarm_r" || BoneName == "hand_r") // 오른팔( )
			{
				if (HitCharacter)
				{
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "thigh_l" || BoneName == "calf_l" || BoneName == "foot_l") // 왼쪽다리( )
			{
				if (HitCharacter)
				{
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			if (BoneName == "thigh_r" || BoneName == "calf_r" || BoneName == "foot_r") // 오른쪽다리( )
			{
				if (HitCharacter)
				{
					HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
					HitCharacter->TakeDamageBone(true, BoneName, FDamageEvent(), GetInstigatorController(), this);
				}
			}
			//else // 그외
			//{
			//	if (HitCharacter)
			//	{
			//		HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
			//		
			//	}
			//}
		}
		
		
	}
	
}

void APortfolio_Tile::OtherDamage(FHitResult HitResult_Other)
{
	float Damage = 100;
	FVector ImpactPoint_Other = HitResult_Other.ImpactPoint;
	HitResult_Other.GetActor();
	UPrimitiveComponent* HitComponent_Other = HitResult_Other.GetComponent();

	FName BoneName_Other = HitResult_Other.BoneName; // 바디 콜리전의 본 이름을 가져옵니다.
	
	APortfolio_GlobalCharacter* HitCharacter_Other = Cast<APortfolio_GlobalCharacter>(HitResult_Other.GetActor());

	if (BoneName_Other == "Head") // 헤드샷( +75% )
	{
		if (HitCharacter_Other)
		{
			float ExtraDamage = Damage * 0.75;
			Damage += ExtraDamage;
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "pelvis" || BoneName_Other == "spine_01" || BoneName_Other == "spine_02") // 몸통샷( +20% )
	{
		if (HitCharacter_Other)
		{
			float ExtraDamage = Damage * 0.20;
			Damage += ExtraDamage;
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "upperarm_l" || BoneName_Other == "lowerarm_l" || BoneName_Other == "hand_l") // 왼팔( )
	{
		if (HitCharacter_Other)
		{
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "upperarm_r" || BoneName_Other == "lowerarm_r" || BoneName_Other == "hand_r") // 오른팔( )
	{
		if (HitCharacter_Other)
		{
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "thigh_l" || BoneName_Other == "calf_l" || BoneName_Other == "foot_l") // 왼쪽다리( )
	{
		if (HitCharacter_Other)
		{
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	if (BoneName_Other == "thigh_r" || BoneName_Other == "calf_r" || BoneName_Other == "foot_r") // 오른쪽다리( )
	{
		if (HitCharacter_Other)
		{
			HitCharacter_Other->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
		}
	}

}


void APortfolio_Tile::PerformSweep(FVector CameraLoc, FVector CameraForward)
{
	FVector StartLoc = CameraLoc; // 레이저 시작 지점.
	FVector EndLoc = CameraLoc + (CameraForward * 5000.0f); // 레이저 끝나는 지점.

	FHitResult HitResult; // 히트 결과를 저장할 변수

	// 스위핑 실행
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,         // 히트 결과를 저장할 변수
		StartLoc,    // 스위핑의 시작 위치
		EndLoc,      // 스위핑의 종료 위치
		FQuat::Identity,  // 회전 정보 (기본값 사용)
		ECC_Visibility,   // 채널 (이 예제에서는 가시성 채널 사용)
		FCollisionShape::MakeSphere(100.0f),//MakeBox(FVector(50.0f, 50.0f, 50.0f)), // 스위핑 형태 (박스 형태 사용, 크기 조정 가능)
		FCollisionQueryParams::DefaultQueryParam, // 쿼리 매개변수
		FCollisionResponseParams::DefaultResponseParam // 응답 매개변수
	);

	if (bHit)
	{
		// 스위핑이 어떤 객체와 충돌한 경우
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			// HitActor와 상호작용 또는 다른 작업을 수행할 수 있습니다.
			// 예를 들어 로그를 출력하거나 해당 액터를 파괴할 수 있습니다.
			UE_LOG(LogTemp, Warning, TEXT("스위핑이 %s와 충돌했습니다."), *HitActor->GetName());
		}
		/*
		float Damage = 1000;
		APortfolio_GlobalCharacter* HitCharacter = Cast<APortfolio_GlobalCharacter>(HitResult.GetActor());
		if (HitCharacter)
		{
			HitCharacter->TakeDamage(Damage, FDamageEvent(), GetInstigatorController(), this);
			int a = 0;
		}
		*/
	}

}




