// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "TPSPlayer.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class MAGICIAN_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	class USpringArmComponent* SpringArmCom;

	UPROPERTY(VisibleAnywhere, Category="Camera")
	class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* MoveIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* LookUpIA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputAction* TurnIA;


	void Move(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Turn(const FInputActionValue& Value);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FVector MoveDirection;

public:
	UPROPERTY(EditAnywhere, Category="Move")
	float MoveSpeed;
};
