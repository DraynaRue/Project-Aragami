// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	TotalAmmo = StartAmmo;
	RoundsInMag = BaseMagazine * MagazineMod;
	TotalAmmo -= RoundsInMag;

	isFiring = false;
	canFire = true;

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));	
}

void APlayerCharacter::StartFiring()
{
	isFiring = true;

	if (RoundsInMag <= 0 && isReloading == false)
	{
		Reload();
	}
}

void APlayerCharacter::StopFiring()
{
	isFiring = false;
}

void APlayerCharacter::Reload()
{
	isReloading = true;
	canFire = false;

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		int mag = BaseMagazine * MagazineMod;

		if (RoundsInMag < mag && TotalAmmo > 0)
		{
			float rld = BaseReload * ReloadMod;
			GetWorldTimerManager().SetTimer(ReloadTimer_TimerHandle, this, &APlayerCharacter::ReloadTimer_Expired, rld, false);
		}
		else if (RoundsInMag == mag || TotalAmmo <= 0)
		{
			isReloading = false;
			canFire = true;
		}
	}
}

void APlayerCharacter::FireRateTimer_Expired()
{
	canFire = true;
}

void APlayerCharacter::ReloadTimer_Expired()
{
	int mag = BaseMagazine * MagazineMod;

	mag -= RoundsInMag;
	if (TotalAmmo - mag >= 0)
	{
		TotalAmmo -= mag;
		RoundsInMag = BaseMagazine * MagazineMod;
	}
	else if (TotalAmmo - mag < 0)
	{
		RoundsInMag += TotalAmmo;
		TotalAmmo = 0;
	}

	isReloading = false;
	canFire = true;
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
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float dmg = BaseDamage * DamageMod;
	float rld = BaseReload * ReloadMod;
	float frrt = BaseFireRate * FireRateMod;

	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		ReloadTime = GetWorldTimerManager().GetTimerElapsed(ReloadTimer_TimerHandle);

		if (isFiring == true && isReloading == false)
		{
			if (canFire && RoundsInMag > 0)
			{
				FHitResult OutHit;
				FVector Start = FP_Gun->GetComponentLocation();

				FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
				FVector End = ((ForwardVector * 10000.0f) + Start);
				FCollisionQueryParams CollisionParams;

				DrawDebugLine(World, Start, End, FColor::Green, true);

				bool isHit = World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

				RoundsInMag--;

				canFire = false;

				/*if (RoundsInMag == 0)
				{
					GetWorldTimerManager().SetTimer(FireRateTimer_TimerHandle, this, &APlayerCharacter::ReloadTimer_Expired, rld, false);
				}
				else*/

				GetWorldTimerManager().SetTimer(FireRateTimer_TimerHandle, this, &APlayerCharacter::FireRateTimer_Expired, frrt, false);

				if (isHit)
				{
					if (OutHit.bBlockingHit)
					{
						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("You Are Hitting: %s"), *OutHit.GetActor()->GetName()));
						}
					}
				}


				// try and play the sound if specified
				if (FireSound != NULL)
				{
					UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
				}

				// try and play a firing animation if specified
				if (FireAnimation != NULL)
				{
					// Get the animation object for the arms mesh
					UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
					if (AnimInstance != NULL)
					{
						AnimInstance->Montage_Play(FireAnimation, 1.f);
					}
				}
			}
		}
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

	// Bind fire events
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::StopFiring);

	// Bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::Reload);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// Bind camera rotations
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

