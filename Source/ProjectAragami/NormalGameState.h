// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NormalGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARAGAMI_API ANormalGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// Modifier added to BaseDamage
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float DamageMod;

	// Modifier added to BaseReload
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float ReloadMod;

	// Modifier added to BaseFireRate
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float FireRateMod;

	// Modifier added to BaseMagazine
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
	float MagazineMod;
	
};
