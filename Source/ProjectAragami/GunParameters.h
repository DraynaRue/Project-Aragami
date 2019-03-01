// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"
#include "GunParameters.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTARAGAMI_API AGunParameters : public AInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Category = GunParameters, EditAnywhere, BluepritnReadWrite)
	float BaseDamage;

	UPROPERTY(Category = GunParameters, EditAnywhere, BluepritnReadWrite)
	float BaseReload;

	UPROPERTY(Category = GunParameters, EditAnywhere, BluepritnReadWrite)
	float BaseFireRate;

	UPROPERTY(Category = GunParameters, EditAnywhere, BluepritnReadWrite)
	float BaseMagSize;
};
