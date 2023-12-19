// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_AnimInstance.h"
#include "Portfolio_Character.h"


/*
void UPlayer_AnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	// 나자신이 애님인스턴스입니다.
	OnMontageEnded.AddDynamic(this, &UPlayer_AnimInstance::MontageEnd);

	// 아이들       => 공격
	// 브랜드 아웃     블랜드인

	//OnMontageBlendingOut.AddDynamic(this, &UPlayer_AnimInstance::MontageEnd);

	// 나를 가진 액터를 여기서 가져옵니다.
	// TPS캐릭터에게 다 세팅을 해줬다.

	APortfolio_Character* Chracter = Cast<APortfolio_Character>(GetOwningActor());

	if (nullptr == Chracter && false == Chracter->IsValidLowLevel())
	{
		return;
	}

	AllAnimations = Chracter->AllAnimations;
}

// 시작할때 한번 업데이트를 돌려주므로.
void UPlayer_AnimInstance::NativeUpdateAnimation(float _DeltaTime)
{
	Super::NativeUpdateAnimation(_DeltaTime);

	if (0 == AllAnimations.Num())
	{
		return;
	}

	APortfolio_Character* Chracter = Cast<APortfolio_Character>(GetOwningActor());

	if (nullptr == Chracter && false == Chracter->IsValidLowLevel())
	{
		return;
	}

	AniState = Chracter->AniState;

	class UAnimMontage* Montage = AllAnimations[AniState];

	if (nullptr == Montage)
	{
		return;
	}

	if (false == Montage_IsPlaying(Montage))
	{
		Montage_Play(Montage, 1.0f);
	}
}

void UPlayer_AnimInstance::MontageEnd(UAnimMontage* Anim, bool _Inter)
{
	//TSubclassOf<UPlayer_AnimInstance> Inst = UPlayer_AnimInstance::StaticClass();

	APortfolio_Character* Chracter = Cast<APortfolio_Character>(GetOwningActor());

	if (nullptr == Chracter && false == Chracter->IsValidLowLevel())
	{
		return;
	}

	// Anim 종료된 몽타주


	if (AllAnimations[EAniState::W_Attack] == Anim)
	{
		if (AniState == EAniState::Idle)
		{
			return;
			//AniState = EAniState::Idle;
			//Chracter->AniState = AniState;
			//Montage_Play(AllAnimations[EAniState::Idle], 1.0f);
		}
		else
		{

			AniState = EAniState::W_Aiming;
			Chracter->AniState = AniState;
			Montage_Play(AllAnimations[EAniState::W_Aiming], 1.0f);
		}
	}


	if (AllAnimations[EAniState::CrouchOn] == Anim)
	{
		if (AniState == EAniState::Crouch_Idle)
		{
			return;
		}
		else
		{
			AniState = EAniState::Crouch_Idle;
			Chracter->AniState = AniState;
			Montage_Play(AllAnimations[EAniState::Crouch_Idle], 1.0f);
		}
	}

	if (AllAnimations[EAniState::CrouchOff] == Anim)
	{
		if (AniState == EAniState::Idle)
		{
			return;
		}
		else
		{

			AniState = EAniState::Idle;
			Chracter->AniState = AniState;
			Montage_Play(AllAnimations[EAniState::Idle], 1.0f);
		}
	}
}
*/