// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalProjectile.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PROJECTARAGAMI_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Camera component
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* PlayerCameraComponent;

	//UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<NormalProjectile>* AmmoType;

protected:
	float startTime;
	float currentTime;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// handles lvoving forwards / backwards
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
};
