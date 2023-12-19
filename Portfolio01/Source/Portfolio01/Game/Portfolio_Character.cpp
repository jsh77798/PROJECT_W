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

	//ĳ���� �⺻ �̵��ӵ� ����
	MoveCom = Cast<UCharacterMovementComponent>(GetMovementComponent());
	MoveCom->MaxWalkSpeed = 280.0f; 
	
	//Create our components �������� ����
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->SetupAttachment(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 140.0f;
	OurCameraSpringArm->SocketOffset.Y = 55.0f;
	OurCameraSpringArm->SocketOffset.Z = 65.0f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 10.0f;

	//ī�޶� ����(�տ� �������Ͽ� �ٿ���)
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(OurCameraSpringArm);

	
	//Take control of the default Player
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	//ĳ���� �̵� ȸ�� (#include "GameFramework/CharacterMovementComponent.h" ��� �ʿ�)
	//GetCharacterMovement()->bOrientRotationToMovement = true;
	//GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f);
}

// Called when the game starts or when spawned
void APortfolio_Character::BeginPlay()
{
	SetAllAnimation(MapAnimation);

	Super::BeginPlay();

	
	// PlayerData���� ���ݷ��� ������ �����Ѵ�
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

	//Run ����Ȯ�� �� ����
	{
	    if (AniStateValue == EAniState::Idle)
	    {
	    	RunCheck = 0;
			RunZooming = false;
	    }
	}

	//use controller rotation yaw ����
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
		//FMath::Lerp<float>(Af, Bf, C) -> C�� �ӷ����� A-B�� �Ѵ�.
		OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(140.0f, A, ZoomFactor);
		OurCameraSpringArm->SocketOffset.Y = FMath::Lerp<float>(55.0f, B, ZoomFactor);
		OurCameraSpringArm->SocketOffset.Z = FMath::Lerp<float>(65.0f, C, ZoomFactor);
	}

	/*
	UPortfolio_Widget_Inventory* MyWidget = CreateWidget<UPortfolio_Widget_Inventory>(GetWorld(), UPortfolio_Widget_Inventory::StaticClass());
	if (MyWidget)
	{
		// �������� Ŭ������ ���� ����
		//MyWidget->MyVariable = 42;

		// �������� Ŭ������ ���� �б�
		int32 WidgetVariableValue = MyWidget->MyVariable;
		_Bullet += WidgetVariableValue;
	}
	*/
	//���ݿ��� AttackCheck == 1�� �Ǹ� ���⼭ ���ݽ���
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

	// Anim ����� ��Ÿ��
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

		// �����
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

	// Ű�� �Լ��� �����Ѵ�
	// �ش�Ű�� ������ �� �Լ��� �����Ų��
	// BindAction_���϶��� �ѹ� �����Ų��
	// BindAxis_���϶��� ��� �����Ų��
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

//�¿��̵�
void APortfolio_Character::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();
			// transform to world space and add it
			// ���� �� ȸ���� �����ͼ� y�࿡ �ش��ϴ� �຤�͸� ������ �̴ϴ�.
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);

			//�޸��� �̵�
			if (RunCheck == 1 && AimingActionCheck == 0 && AniStateValue != EAniState::Idle)
			{
				MoveCom->MaxWalkSpeed = 360.0f;
				RunZooming = true;
				SetAniState(Val > 0.f ? EAniState::RightRun : EAniState::LeftRun);
				return;
			}
			
			//��ũ���� �̵�
			if (CrouchCheck == 1)
			{
				MoveCom->MaxWalkSpeed = 200.0f;
				SetAniState(Val > 0.f ? EAniState::Crouch_RightMove : EAniState::Crouch_LeftMove);
				return;
			}

			//�����̵� �� �⺻�̵�
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

//�յ��̵�
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

			//�޸��� �̵�
			if (RunCheck == 1 && AimingActionCheck == 0 && AniStateValue != EAniState::Idle)
			{
				MoveCom->MaxWalkSpeed = 360.0f;
				RunZooming = true;
				SetAniState(Val > 0.f ? EAniState::ForwardRun : EAniState::BackwardRun);
				return;
			}

			//��ũ���� �̵�
			if (CrouchCheck == 1)
			{
				MoveCom->MaxWalkSpeed = 200.0f;
				SetAniState(Val > 0.f ? EAniState::Crouch_ForwardMove : EAniState::Crouch_BackwardMove);
				return;
			}

			//�����̵� �� �⺻�̵�
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
					float a = 60.0f; //Ÿ�پ� ����
					float b = 80.0f; //Ÿ�پ� Y��
					float c = 70.0f; //Ÿ�پ� Z��
					float s = 0.15f; //�ӷ�
					ZoomCheck(&a, &b, &c, &s); //Ÿ�پ� ����
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

//����
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

//����
void APortfolio_Character::IN_AimingAction()
{
	SetAniState(EAniState::W_Aiming);

	SetAimingCheck(true);

	float a = 80.0f; //Ÿ�پ� ����
	float b = 80.0f; //Ÿ�پ� Y��
	float c = 70.0f; //Ÿ�پ� Z��
	float s = 0.15f; //�ӷ�
	ZoomCheck(&a, &b, &c, &s); //Ÿ�پ� ����

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

//����
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

		//Tick���� ����
		AttackCheck = 1;

	}
	return;
}

//����Ʈ(����)
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

		// �߻�ü �����
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

//�޸���
void APortfolio_Character::Run() 
{
	float a = 145.0f; //Ÿ�پ� ����
	float b = 45.0f; //Ÿ�پ� Y��
	float c = 60.0f; //Ÿ�Ͼ� Z��
	float s = 0.35f; //�ӷ�

	if (RunCheck == 0)
	{
		CrouchCheck = 0;
		CrouchZooming = false;

		ZoomCheck(&a, &b, &c, &s); //Ÿ�پ� ����
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

//��ũ����
void APortfolio_Character::Crouch() 
{
	float a = 150.0f; //Ÿ�پ� ����
	float b = 55.0f; //Ÿ�پ� Y��
	float c = 50.0f; //Ÿ�Ͼ� Z��
	float s = 0.35f; //�ӷ�

	if (CrouchCheck == 0 && AimingActionCheck == 0)
	{
		RunCheck = 0;
		AimingActionCheck = 0;
		bZoomingIn = false;

		ZoomCheck(&a, &b, &c, &s); //Ÿ�پ� ����
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
	FVector StartLoc = CameraLoc; // ������ ���� ����.
	FVector EndLoc = CameraLoc + (CameraForward * 5000.0f); // ������ ������ ����.


	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // ��Ʈ ������ ������Ʈ ������.
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

	TArray<AActor*> IgnoreActors; // ������ ���͵�.

	FHitResult HitResult; // ��Ʈ ��� �� ���� ����.

	bool Result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		StartLoc,
		EndLoc,
		ObjectTypes,
		false,
		IgnoreActors, // ������ ���� ���ٰ��ص� null�� ���� �� ����.
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		// ���� �ؿ� 3���� �⺻ ������ ������. �ٲٷ��� ������ ��.
		//, FLinearColor::Red
		//, FLinearColor::Green
		//, 5.0f
	);

	if (Result == true)
	{
		FVector ImpactPoint = HitResult.ImpactPoint;
		// HitResult���� �ʿ��� ������ ����ϸ� ��.
	}
}
*/