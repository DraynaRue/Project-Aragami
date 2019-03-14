// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunParameters.h"
#include "GameFramework/Actor.h"
#include "NormalGun.generated.h"

UCLASS()
class PROJECTARAGAMI_API ANormalGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANormalGun();

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	class TSubclassOf<AGunParameters> GunParameters;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void FireGun();
	
};
