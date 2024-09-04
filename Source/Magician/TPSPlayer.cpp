// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈 메쉬를 설정하기 위해 메쉬 리소스를 찾기
	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/MyResource/UnityChan/unitychan.unitychan'"));

	if(InitMesh.Succeeded())
	{
		// 메쉬를 설정
		GetMesh()->SetSkeletalMesh(InitMesh.Object);

		// 메쉬의 위치와 회전을 조정
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88),FRotator(0,-90,0));
	}

	//Springarm Component , SpringArmComponent를 생성하고 루트 컴포넌트에 부착합니다.
	SpringArmCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmCom->SetupAttachment(RootComponent);
	SpringArmCom->SetRelativeLocationAndRotation(FVector(0,0,50),FRotator(-20,0,0));
	SpringArmCom->TargetArmLength = 530;
	SpringArmCom->bUsePawnControlRotation = true;

	//Camera Component , CameraComponent를 생성하고 스프링암에 부착합니다.
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmCom);
	CameraComp->bUsePawnControlRotation = false;

	// 캐릭터가 Yaw 회전을 컨트롤러에 의해 제어되도록 설정
	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}
	
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveIA,ETriggerEvent::Triggered, this, &ATPSPlayer::Move);
		EnhancedInputComponent->BindAction(LookUpIA,ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);
		EnhancedInputComponent->BindAction(TurnIA,ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
		EnhancedInputComponent->BindAction(JumpIA,ETriggerEvent::Triggered, this, &ATPSPlayer::InputJump);
	}
}

void ATPSPlayer::Move(const FInputActionValue& Value)
{
	// 입력 값을 FVector 형태로 가져옵니다.
	const FVector _CurrentValue = Value.Get<FVector>();
	if(Controller)
	{
		// MoveDirection 벡터에 입력된 X, Y 값을 설정합니다.
		MoveDirection.Y = _CurrentValue.X; // 좌우 방향 값
		MoveDirection.X = _CurrentValue.Y; // 전후 방향 값
	}

	// 카메라의 현재 회전에 맞춰 MoveDirection을 변환합니다.
	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);
	// 변환된 MoveDirection 방향으로 캐릭터를 이동시킵니다.
	AddMovementInput(MoveDirection);
	// MoveDirection을 초기화하여 다음 프레임에서 누적되지 않도록 합니다.
	MoveDirection = FVector::ZeroVector;
}

void ATPSPlayer::LookUp(const FInputActionValue& Value)
{
	// 입력 값을 float 형태로 가져옵니다.
	float _CurrentValue;

	if(isInvertLookUp)
	{
		_CurrentValue = Value.Get<float>() * -1;
	}
	else
	{
		_CurrentValue = Value.Get<float>();
	}

	// 카메라의 Pitch 값을 변경하여 위아래 시점 이동을 수행합니다.
	AddControllerPitchInput(_CurrentValue);
}

void ATPSPlayer::Turn(const FInputActionValue& Value)
{
	// 입력 값을 float 형태로 가져옵니다.
	const float _CurrentValue = Value.Get<float>();
	// 카메라의 Yaw 값을 변경하여 좌우 시점 이동을 수행합니다.
	AddControllerYawInput(_CurrentValue);	
}

void ATPSPlayer::InputJump(const FInputActionValue& Value)
{
	Jump();
}

