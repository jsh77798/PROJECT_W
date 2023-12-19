// Fill out your copyright notice in the Description page of Project Settings.


#include "Portfolio_Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/PrimitiveComponent.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include <UI/Portfolio_Widget_Inventory.h>

// Sets default values
APortfolio_Character::APortfolio_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseTurnRate = 25.f;
	BaseLookUpRate = 25.f;

	//캐릭터 기본 이동속도 설정
	MoveCom = Cast<UCharacterMovementComponent>(GetMovementComponent());
	MoveCom->MaxWalkSpeed = 280.0f; 
	
	//Create our components 스프링암 설정
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 140.0f;
	OurCameraSpringArm->SocketOffset.Y = 55.0f;
	OurCameraSpringArm->SocketOffset.Z = 65.0f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 10.0f;

	//카메라 설정(앞에 스프링암에 붙여줌)
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(OurCameraSpringArm);

	
	//Take control of the default Player
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	//캐릭터 이동 회전 (#include "GameFramework/CharacterMovementComponent.h" 헤더 필요)
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

// Called when the game starts or when spawned
void APortfolio_Character::BeginPlay()
{
	SetAllAnimation(MapAnimation);

	Super::BeginPlay();

	
	// PlayerData에서 공격력을 가져와 저장한다
	UPortfolio_GameInstance* Inst = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();
	if (nullptr != Inst)
	{
		CurPlayerData = Inst->GetPlayerData(PlayerDataName);
	}
	PlayerAtt = CurPlayerData->ATT;

	CurPlayerData->Bullet = 5;
	Damage(PlayerAtt);

	GetGlobalAnimInstance()->OnMontageBlendingOut.AddDynamic(this, &APortfolio_Character::MontageEnd);
	GetGlobalAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &APortfolio_Character::AnimNotifyBegin);
	SetAniState(EAniState::Idle);
}

// Called every frame
void APortfolio_Character::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);
	CameraLoc = camera->GetComponentLocation();
	CameraForward = camera->GetForwardVector();

	AniStateValue = GetAniState<EAniState>();

	Bullet = CurPlayerData->Bullet;
	LoadBullet = CurPlayerData->LoadBullet;
	
	if (GetItem == true)
	{
		GetitemCheck = true;
	}

	if (GetitemCheck == true && Item_Get_InputCheck == true)
	{
		CurPlayerData->Bullet += 1;
		Item_Get_InputCheck = false;
	}
	else 
	{
	    Item_Get_InputCheck = false;
	}

	{
        AttackHitCheck = GetHit();
		if (AttackHitCheck == true) 
		{
			SetAniState(EAniState::Hit);
			SetAttackAnimcheck(false);
		}
	}

	//Run 상태확인 및 설정
	{
	    if (AniStateValue == EAniState::Idle)
	    {
	    	RunCheck = 0;
			RunZooming = false;
	    }
	}

	//use controller rotation yaw 설정
	{
		if (AniStateValue == EAniState::Idle || AniStateValue == EAniState::ForwardMove || RunCheck == 1)
		{
			bUseControllerRotationYaw = false;

			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
		}
		else 
		{
		    bUseControllerRotationYaw = true;
		}
	}

	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		if (bZoomingIn || RunZooming || CrouchZooming)
		{
			ZoomFactor += DeltaTime / S;         //Zoom in over half a second
		}
		else
		{
			ZoomFactor -= DeltaTime / S;        //Zoom out over a quarter of a second
		}
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		//FMath::Lerp<float>(Af, Bf, C) -> C의 속력으로 A-B를 한다.
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(140.0f, A, ZoomFactor);
		OurCameraSpringArm->SocketOffset.Y = FMath::Lerp<float>(55.0f, B, ZoomFactor);
		OurCameraSpringArm->SocketOffset.Z = FMath::Lerp<float>(65.0f, C, ZoomFactor);
	}

	/*
	UPortfolio_Widget_Inventory* MyWidget = CreateWidget<UPortfolio_Widget_Inventory>(GetWorld(), UPortfolio_Widget_Inventory::StaticClass());
	if (MyWidget)
	{
		// 유저위젯 클래스의 변수 설정
		//MyWidget->MyVariable = 42;

		// 유저위젯 클래스의 변수 읽기
		int32 WidgetVariableValue = MyWidget->MyVariable;
		_Bullet += WidgetVariableValue;
	}
	*/
	//공격에서 AttackCheck == 1이 되면 여기서 공격실행
	{
		if (AttackCheck == 1) {

	        if (AimingActionCheck == 1 && AniStateValue == EAniState::W_Aiming)
	        {
				SetAniState(EAniState::W_Attack);

				//AimingAttack();
				CurPlayerData->LoadBullet -= 1;
			    AttackCheck= 0;
	        }
 
	        if (AimingActionCheck == 1
		        || AniStateValue == EAniState::Aiming_ForwardMove
		        || AniStateValue == EAniState::Aiming_BackwardMove
		        || AniStateValue == EAniState::Aiming_RightMove
		        || AniStateValue == EAniState::Aiming_LeftMove)
	        {
				SetAniState(EAniState::W_Attack);
				AttackCheck= 0;
	        }
		}
	}
}

void APortfolio_Character::ZoomCheck(float *_A, float *_B, float *_C, float* _S)
{
	A = *_A;
	B = *_B;
	C = *_C;
	S = *_S;
}

void APortfolio_Character::MontageEnd(UAnimMontage* Anim, bool _Inter)
{
	TSubclassOf<UAnimInstance> Inst = APortfolio_Character::StaticClass();

	// Anim 종료된 몽타주
	if (MapAnimation[EAniState::W_Attack] == Anim)
	{
		if (AniStateValue == EAniState::Idle)
		{
			return;
		}
		else
		{
			SetAniState(EAniState::W_Aiming);
		}
	}

	if (MapAnimation[EAniState::CrouchOn] == Anim)
	{
		if (AniStateValue == EAniState::Crouch_Idle)
		{
			return;
		}
		else
		{
			SetAniState(EAniState::Crouch_Idle);
		}
	}

	if (MapAnimation[EAniState::CrouchOff] == Anim)
	{
		if (AniStateValue == EAniState::Idle)
		{
			return;
		}
		else
		{

			SetAniState(EAniState::Idle);
		}
	}

	if (MapAnimation[EAniState::Hit] == Anim)
	{
		if (AniStateValue == EAniState::Idle)
		{
			return;
		}
		else
		{

			SetAniState(EAniState::Idle);
		}
	}
}

// Called to bind functionality to input
void APortfolio_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	static bool bBindingsAdded = false;

	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		// 축매핑
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::S, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::D, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerTurn", EKeys::MouseX, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerLookUp", EKeys::MouseY, -1.f));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("item_Get"), EKeys::X));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerLoad"), EKeys::R));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerAiming"), EKeys::RightMouseButton));
    	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerAttack"), EKeys::LeftMouseButton));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerRun"), EKeys::LeftShift));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerCrouch"), EKeys::Q));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("StatusWindow"), EKeys::Zero));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("InventoryWindow"), EKeys::Nine));
	}

	// 키와 함수를 연결한다
	// 해당키가 눌리면 이 함수를 실행시킨다
	// BindAction_축일때는 한번 실행시킨다
	// BindAxis_축일때는 계속 실행시킨다
	PlayerInputComponent->BindAxis("PlayerMoveForward", this, &APortfolio_Character::MoveForward);
	PlayerInputComponent->BindAxis("PlayerMoveRight", this, &APortfolio_Character::MoveRight);
	PlayerInputComponent->BindAxis("PlayerTurn", this, &APortfolio_Character::AddControllerYawInput);
	PlayerInputComponent->BindAxis("PlayerTurnRate", this, &APortfolio_Character::TurnAtRate);
	PlayerInputComponent->BindAxis("PlayerLookUp", this, &APortfolio_Character::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("PlayerLookUpRate", this, &APortfolio_Character::LookUpAtRate);

	PlayerInputComponent->BindAction("item_Get", EInputEvent::IE_Pressed, this, &APortfolio_Character::item_Get_check);

	PlayerInputComponent->BindAction("PlayerLoad", EInputEvent::IE_Pressed, this, &APortfolio_Character::LoadAction);
	PlayerInputComponent->BindAction("PlayerAiming", EInputEvent::IE_Pressed , this, &APortfolio_Character::IN_AimingAction);
	PlayerInputComponent->BindAction("PlayerAiming", EInputEvent::IE_Released, this, &APortfolio_Character::OUT_AimingAction);
	PlayerInputComponent->BindAction("PlayerAttack", EInputEvent::IE_Pressed, this, &APortfolio_Character::AttackAction);

	PlayerInputComponent->BindAction("PlayerRun", EInputEvent::IE_Pressed, this, &APortfolio_Character::Run);
	PlayerInputComponent->BindAction("PlayerCrouch", EInputEvent::IE_Pressed, this, &APortfolio_Character::Crouch);
	
}

//좌우이동
void APortfolio_Character::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();
			// transform to world space and add it
			// 현재 내 회전을 가져와서 y축에 해당하는 축벡터를 얻어오는 겁니다.
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);

			//달리기 이동
			if (RunCheck == 1 && AimingActionCheck == 0 && AniStateValue != EAniState::Idle)
			{
				MoveCom->MaxWalkSpeed = 360.0f;
				RunZooming = true;
				SetAniState(Val > 0.f ? EAniState::RightRun : EAniState::LeftRun);
				return;
			}
			
			//웅크리기 이동
			if (CrouchCheck == 1)
			{
				MoveCom->MaxWalkSpeed = 200.0f;
				SetAniState(Val > 0.f ? EAniState::Crouch_RightMove : EAniState::Crouch_LeftMove);
				return;
			}

			//조준이동 및 기본이동
		    {
			    if (AimingActionCheck == 0) 
			    {
			  	    MoveCom->MaxWalkSpeed = 280.0f;
					SetAniState(Val > 0.f ? EAniState::RightMove : EAniState::LeftMove);
				    return;
			    }
			    else 
			    {
			    	MoveCom->MaxWalkSpeed = 230.0f;
					SetAniState(Val > 0.f ? EAniState::Aiming_RightMove : EAniState::Aiming_LeftMove);
				    return;
		  	    }  
		    }


		}
	}
	else
	{
		if (AniStateValue == EAniState::RightMove || AniStateValue == EAniState::LeftMove )
		{
			SetAniState(EAniState::Idle);
		}
		if (AniStateValue == EAniState::Aiming_RightMove || AniStateValue == EAniState::Aiming_LeftMove )
		{
			SetAniState(EAniState::W_Aiming);
		}

		if (AniStateValue == EAniState::RightRun || AniStateValue == EAniState::LeftRun)
		{
			SetAniState(EAniState::Idle);
			RunZooming = false;
			RunCheck = 0;
		}

		if (AniStateValue == EAniState::Crouch_RightMove || AniStateValue == EAniState::Crouch_LeftMove)
		{
			SetAniState(EAniState::Crouch_Idle);
		}
	}
}

//앞뒤이동
void APortfolio_Character::MoveForward(float Val)
{
	if (Val != 0.f)
	{

		if (Controller)
		{
			
			FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
			AddMovementInput(Direction, Val);

			//달리기 이동
			if (RunCheck == 1 && AimingActionCheck == 0 && AniStateValue != EAniState::Idle)
			{
				MoveCom->MaxWalkSpeed = 360.0f;
				RunZooming = true;
				SetAniState(Val > 0.f ? EAniState::ForwardRun : EAniState::BackwardRun);
				return;
			}

			//웅크리기 이동
			if (CrouchCheck == 1)
			{
				MoveCom->MaxWalkSpeed = 200.0f;
				SetAniState(Val > 0.f ? EAniState::Crouch_ForwardMove : EAniState::Crouch_BackwardMove);
				return;
			}

			//조준이동 및 기본이동
			{
				if (AimingActionCheck == 0)
				{
					MoveCom->MaxWalkSpeed = 280.0f;
					SetAniState(Val > 0.f ? EAniState::ForwardMove : EAniState::BackwardMove);
					return;
				}
				else
				{
					MoveCom->MaxWalkSpeed = 230.0f;
					float a = 60.0f; //타겟암 길이
					float b = 80.0f; //타겟암 Y축
					float c = 70.0f; //타겟암 Z축
					float s = 0.15f; //속력
					ZoomCheck(&a, &b, &c, &s); //타겟암 설정
					SetAniState(Val > 0.f ? EAniState::Aiming_ForwardMove : EAniState::Aiming_BackwardMove);
					return;
				}
			}

		}
	}
	else
	{
		if (AniStateValue == EAniState::ForwardMove || AniStateValue == EAniState::BackwardMove )
		{
			SetAniState(EAniState::Idle);
		}
		if (AniStateValue == EAniState::Aiming_ForwardMove || AniStateValue == EAniState::Aiming_BackwardMove)
		{
			SetAniState(EAniState::W_Aiming);
		}

		if (AniStateValue == EAniState::ForwardRun || AniStateValue == EAniState::BackwardRun)
		{
			SetAniState(EAniState::Idle);
			RunZooming = false;
			RunCheck = 0;
		}

		if (AniStateValue == EAniState::Crouch_ForwardMove || AniStateValue == EAniState::Crouch_BackwardMove)
		{
			SetAniState(EAniState::Crouch_Idle);
		}
	}
}

void APortfolio_Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void APortfolio_Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

//장전
void APortfolio_Character::LoadAction()
{
	int _LoadBullet = 6 - CurPlayerData->LoadBullet;
	if (CurPlayerData->LoadBullet <= 5)
	{
		if (CurPlayerData->Bullet != 0) 
		{
		    SetAniState(EAniState::Load);

			//_LoadBullet = 6 - _LoadBullet ;
			//_LoadBullet *= -1;
			for (int i = 1; i < _LoadBullet; i++)
			{
				if (CurPlayerData->Bullet != 0)
				{
				    CurPlayerData->LoadBullet += 1;
					CurPlayerData->Bullet -= 1;
				}
			}
		}
	}

}

//조준
void APortfolio_Character::IN_AimingAction()
{
	SetAniState(EAniState::W_Aiming);

	SetAimingCheck(true);

	float a = 80.0f; //타겟암 길이
	float b = 80.0f; //타겟암 Y축
	float c = 70.0f; //타겟암 Z축
	float s = 0.15f; //속력
	ZoomCheck(&a, &b, &c, &s); //타겟암 설정

	RunCheck = 0;
	CrouchCheck = 0;
	CrouchZooming = false;

	AimingActionCheck = 1;
	//ZoomingIn = 1;
	bZoomingIn = true;
}

void APortfolio_Character::OUT_AimingAction()
{
	if (AimingActionCheck == 0)
	{
		SetAniState(EAniState::Idle);
	}
	else
	{
		SetAniState(EAniState::Idle);
		AimingActionCheck = 0;
	}

	SetAimingCheck(false);
	bZoomingIn = false;
}

//공격
void APortfolio_Character::AttackAction()
{
	/*
	if ( AimingActionCheck == 1 && AniState == EAniState::W_Aiming )
	{
	    AniState = EAniState::W_Attack;
	}

	if (AimingActionCheck == 1 
		|| AniState == EAniState::Aiming_ForwardMove
		|| AniState == EAniState::Aiming_BackwardMove
		|| AniState == EAniState::Aiming_RightMove
		|| AniState == EAniState::Aiming_LeftMove )
	{
		AniState = EAniState::W_Attack;
    }
	*/

	int _LoadBullet = CurPlayerData->LoadBullet;
	if (_LoadBullet != 0 ) 
	{

		//Tick에서 공격
		AttackCheck = 1;

	}
	return;
}

//이펙트(공격)
void APortfolio_Character::AnimNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	UPortfolio_GameInstance* Inst = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();

	TSubclassOf<UObject> Effect = Inst->GetSubClass(TEXT("ShotEffect"));
	TSubclassOf<UObject> ShellEffect = Inst->GetSubClass(TEXT("ShellEffect"));
	TSubclassOf<UObject> RangeAttack = Inst->GetSubClass(TEXT("PlayerRangeAttack"));

	/*
	if (nullptr != Effect)
	{
		AActor* Actor = GetWorld()->SpawnActor<AActor>(Effect);
		Actor->SetActorLocation(GetActorLocation());
		Actor->SetActorRotation(GetActorRotation());
	}
	*/
	if (nullptr != Effect)
	{
		FTransform Trans;
		FVector Pos;
		TArray<UActorComponent*> MeshEffects = GetComponentsByTag(USceneComponent::StaticClass(), TEXT("WeaponEffect"));
		TArray<UActorComponent*> StaticMeshs = GetComponentsByTag(USceneComponent::StaticClass(), TEXT("WeaponMesh"));

		USceneComponent* EffectCom = Cast<USceneComponent>(MeshEffects[0]);
		Pos = EffectCom->GetComponentToWorld().GetLocation();

		// 발사체 만들기
		{
			AActor* Actor = GetWorld()->SpawnActor<AActor>(RangeAttack);
			Actor->Tags.Add(TEXT("Damage"));
			APortfolio_Tile* ProjectTile = Cast<APortfolio_Tile>(Actor);
			ProjectTile->SetActorLocation(Pos);
			ProjectTile->SetActorRotation(GetActorRotation());
			ProjectTile->GetSphereComponent()->SetCollisionProfileName(TEXT("PlayerAttack"), true);
			ProjectTile->AimingAttack(CameraLoc, CameraForward);
			ProjectTile->PerformSweep(CameraLoc, CameraForward);
		}
	}
	/*
	if (nullptr != ShellEffect)
	{
		FTransform Trans;
		FVector Pos;
		TArray<UActorComponent*> MeshEffects = GetComponentsByTag(USceneComponent::StaticClass(), TEXT("ShellEffect"));
		TArray<UActorComponent*> StaticMeshs = GetComponentsByTag(USceneComponent::StaticClass(), TEXT("WeaponMesh"));

		USceneComponent* EffectCom = Cast<USceneComponent>(MeshEffects[1]);
		Pos = EffectCom->GetComponentToWorld().GetLocation();
	}
	*/
}

//달리기
void APortfolio_Character::Run() 
{
	float a = 145.0f; //타겟암 길이
	float b = 45.0f; //타겟암 Y축
	float c = 60.0f; //타켓암 Z축
	float s = 0.35f; //속력

	if (RunCheck == 0)
	{
		CrouchCheck = 0;
		CrouchZooming = false;

		ZoomCheck(&a, &b, &c, &s); //타겟암 설정
		RunCheck = 1;
		return;
	}
	else
	{
		RunZooming = false;
		RunCheck = 0;
		return;
	}
}

//웅크리기
void APortfolio_Character::Crouch() 
{
	float a = 150.0f; //타겟암 길이
	float b = 55.0f; //타겟암 Y축
	float c = 50.0f; //타켓암 Z축
	float s = 0.35f; //속력

	if (CrouchCheck == 0 && AimingActionCheck == 0)
	{
		RunCheck = 0;
		AimingActionCheck = 0;
		bZoomingIn = false;

		ZoomCheck(&a, &b, &c, &s); //타겟암 설정
		CrouchZooming = true;
	    CrouchCheck = 1;
		SetAniState(EAniState::CrouchOn);
		return;
	}
	else
	{
		SetAniState(EAniState::CrouchOff);
		CrouchZooming = false;
		CrouchCheck = 0;
		return;
	}
}

void APortfolio_Character::item_Get_check()
{
	Item_Get_InputCheck = true;
}

void APortfolio_Character::AnimationTick()
{

}

/*
void APortfolio_Character::BulletPlus()
{
	CurPlayerData->Bullet += 1;
}
*/

/*
void APortfolio_Character::AimingAttack() 
{

	CameraLoc = camera->GetComponentLocation();
	CameraForward = camera->GetForwardVector();
	FVector StartLoc = CameraLoc; // 레이저 시작 지점.
	FVector EndLoc = CameraLoc + (CameraForward * 5000.0f); // 레이저 끝나는 지점.


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

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors, // 무시할 것이 없다고해도 null을 넣을 수 없다.
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		// 여기 밑에 3개는 기본 값으로 제공됨. 바꾸려면 적으면 됨.
		//, FLinearColor::Red
		//, FLinearColor::Green
		//, 5.0f
	);

	if (Result == true)
	{
		FVector ImpactPoint = HitResult.ImpactPoint;
		// HitResult에서 필요한 값들을 사용하면 됨.
	}
}
*/