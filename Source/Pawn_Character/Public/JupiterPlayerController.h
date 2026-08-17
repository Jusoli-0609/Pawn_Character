// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JupiterPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;


UCLASS()
class PAWN_CHARACTER_API AJupiterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AJupiterPlayerController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* MoveAction;
	// IA_Move

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* LookAction;
	// IA_Look

protected:
	virtual void BeginPlay() override;
};