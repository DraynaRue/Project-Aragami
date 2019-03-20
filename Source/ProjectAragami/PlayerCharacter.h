// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECTARAGAMI_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	// Damage dealt to enemies per hit w/o modifiers
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float BaseDamage;

	// Time between magazines w/o modifiers
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float BaseReload;

	// Time between shots w/o modifiers
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float BaseFireRate;

	// Number of shots before reload w/o modifers
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	int BaseMagazine;

	// Modifier added to BaseDamage
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float DamageMod;

	// Modifier added to BaseReload
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float ReloadMod;

	// Modifier added to BaseFireRate
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRateMod;

	UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadWrite)
	float ReloadTime;

	// Modifier added to BaseMagazine
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MagazineMod;

	// Total ammo held at game start
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	int StartAmmo;

	// Total ammo held at current moment of play
	UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadWrite)
	int TotalAmmo;

	// Current number of rounds in the magazine
	UPROPERTY(Category = Gameplay, VisibleAnywhere, BlueprintReadWrite)
	int RoundsInMag;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector GunOffset;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

protected:
	float startTime;
	float currentTime;

	bool isFiring;
	bool isReloading;
	bool canFire;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle FireRateTimer_TimerHandle;
	FTimerHandle ReloadTimer_TimerHandle;

	void StartFiring();
	void StopFiring();
	void Reload();
	void FireRateTimer_Expired();
	void ReloadTimer_Expired();

	// handles moving forwards / backwards
	void MoveForward(float val);

	// handles moving right / left
	void MoveRight(float val);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};
