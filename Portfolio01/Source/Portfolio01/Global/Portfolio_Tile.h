// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Portfolio_Tile.generated.h"

UCLASS()
class PORTFOLIO01_API APortfolio_Tile : public AActor
{
	GENERATED_BODY()
		
public:	
	// Sets default values for this actor's properties
	
	APortfolio_Tile();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor")
	int AttRange;

	UFUNCTION(BlueprintCallable, Category = "Actor")
	void SetAttRange(const int& _AttRange)
	{
		AttRange = _AttRange;
	}

	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	    int RangeAtt = 0;

	int Data = 0;

	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float Speed = 3000.0f;

	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		float DeathTime = 5.0f;

	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> DeathCreateObject = nullptr;

	UFUNCTION()
		FORCEINLINE USphereComponent* GetSphereComponent()
	{
		return SphereComponent;
	}

		UFUNCTION()
		void PerformSweep(FVector CameraLoc, FVector CameraForward);

		UFUNCTION()
		void AimingAttack(FVector CameraLoc, FVector CameraForward);

		UFUNCTION()
		void OtherDamage(FHitResult HitResult_Other);

		UPROPERTY(EditAnywhere, Category = "Bullet")
		float MaxDamageDistance = 5.0f;

		UPROPERTY(EditAnywhere, Category = "Bullet")
		float DamagePerUnit = 500.0f; 
        
		//UFUNCTION()
		//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

		FVector GetLocation;

		//UFUNCTION()
		//void PerformLineTrace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	//Called every frame
	UFUNCTION()
		void DestroyProjectile(AActor* _Destroy);

	UPROPERTY(Category = "Effect", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* SphereComponent = nullptr;
};
