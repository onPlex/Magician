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

	
	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/MyResource/UnityChan/unitychan.unitychan'"));

	if(InitMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);

		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-88),FRotator(0,-90,0));
	}

	//Springarm Component
	SpringArmCom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmCom->SetupAttachment(RootComponent);
	SpringArmCom->SetRelativeLocationAndRotation(FVector(0,0,50),FRotator(-20,0,0));
	SpringArmCom->TargetArmLength = 530;
	SpringArmCom->bUsePawnControlRotation = true;

	//Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmCom);
	CameraComp->bUsePawnControlRotation = false;

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
		EnhancedInputComponent->BindAction(MoveIA,ETriggerEvent::Triggered, this, &ATPSPlayer::LookUp);
		EnhancedInputComponent->BindAction(MoveIA,ETriggerEvent::Triggered, this, &ATPSPlayer::Turn);
	}
}

void ATPSPlayer::Move(const FInputActionValue& Value)
{
	const FVector _CurrentValue = Value.Get<FVector>();
	if(Controller)
	{
		MoveDirection.Y = _CurrentValue.X;
		MoveDirection.X = _CurrentValue.Y;
	}

	MoveDirection = FTransform(GetControlRotation()).TransformVector(MoveDirection);
	AddMovementInput(MoveDirection);
	MoveDirection = FVector::ZeroVector;
}

void ATPSPlayer::LookUp(const FInputActionValue& Value)
{
	const float _CurrentValue = Value.Get<float>();
	AddControllerPitchInput(_CurrentValue);
}

void ATPSPlayer::Turn(const FInputActionValue& Value)
{
	const float _CurrentValue = Value.Get<float>();
	AddControllerYawInput(_CurrentValue);
}

