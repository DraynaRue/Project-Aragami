// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Enemy.generated.h"

UCLASS()
class PROJECTARAGAMI_API AEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* collisionComponent;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//AI Module
	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	UAIPerceptionComponent* aiPercComp;

	UPROPERTY(VisibleDefaultsOnly, Category = Enemy)
	UAISenseConfig_Sight* sightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	int maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	int currentHealth;

	UPROPERTY(EditAnywhere, Category = Enemy)
	int XpValue;

	UFUNCTION()
	void OnSensed(const TArray<AActor*>& updatedActors);

	void setNewRotation(FVector targPos, FVector currPos);

	void kill();

	UPROPERTY(VisibleAnywhere, Category = Movement)
	FRotator enemyRot;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	FVector baseLoc;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	FVector currVel;

	UPROPERTY(EditAnywhere, Category = Movement)
	float speed;

	float distSq;
	bool backToBase;
};
