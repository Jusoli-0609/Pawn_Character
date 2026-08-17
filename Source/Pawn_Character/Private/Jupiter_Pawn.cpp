// Fill out your copyright notice in the Description page of Project Settings.

#include "Jupiter_Pawn.h"

#include "JupiterPlayerController.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "InputActionValue.h"


// Sets default values
AJupiter_Pawn::AJupiter_Pawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));

	SetRootComponent(CapsuleComp);

	CapsuleComp->InitCapsuleSize(42.0f, 96.0f);
	CapsuleComp->SetSimulatePhysics(false);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 400.0f;
	SpringArmComp->bUsePawnControlRotation = false;


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp,USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}

void AJupiter_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AJupiterPlayerController* PlayerController = Cast<AJupiterPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(PlayerController->MoveAction,ETriggerEvent::Triggered,this,&AJupiter_Pawn::Move);
			}

			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(PlayerController->LookAction,ETriggerEvent::Triggered,this,&AJupiter_Pawn::Look);
			}
		}
	}
}


void AJupiter_Pawn::Move(const FInputActionValue& Value)
{
	FVector2D MoveInput =Value.Get<FVector2D>();

	if (MoveInput.SizeSquared() > 1.0f)
	{
		MoveInput.Normalize();
	}

	const float DeltaTime =GetWorld()->GetDeltaSeconds();
	const FVector LocalOffset(MoveInput.X * MoveSpeed * DeltaTime,MoveInput.Y * MoveSpeed * DeltaTime,0.0f);

	AddActorLocalOffset(LocalOffset,true);
}

void AJupiter_Pawn::Look(const FInputActionValue& Value)
{
	const FVector2D LookInput =Value.Get<FVector2D>();
	const float YawAmount =LookInput.X * LookSensitivity;

	AddActorLocalRotation(FRotator(0.0f,YawAmount,0.0f));

	CurrentPitch +=LookInput.Y * LookSensitivity;
	CurrentPitch = FMath::Clamp(CurrentPitch,MinPitch,MaxPitch);
	SpringArmComp->SetRelativeRotation(FRotator(CurrentPitch,0.0f,0.0f));
}