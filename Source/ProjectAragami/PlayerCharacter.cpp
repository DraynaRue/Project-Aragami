// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	//create a camera component
	PlayerCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	PlayerCameraComponent->SetupAttachment(GetCapsuleComponent());
	PlayerCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f);
	PlayerCameraComponent->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	startTime = FDateTime().Now().GetSecond();
}

void APlayerCharacter::MoveForward(float val)
{
	if (val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), val);
	}
}

void APlayerCharacter::MoveRight(float val)
{
	if (val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), val);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime = FDateTime().Now().GetSecond();

	if (currentTime - startTime >= 1.00f && FDateTime().Now().GetSecond() < 59.00f)
	{
		startTime = FDateTime().Now().GetSecond();
		timeRemaining--;
	}
	else if (FDateTime().Now().GetSecond() >= 59.00f)
	{
		startTime = 1.00f;
	}

	if (timeRemaining <= 0.00f)
	{ 
		UWorld* TheWorld = GetWorld();
		UGameplayStatics::OpenLevel(GetWorld(), "LoseScreen");
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// Bind camera rotations
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

