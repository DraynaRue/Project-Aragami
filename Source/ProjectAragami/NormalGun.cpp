// Fill out your copyright notice in the Description page of Project Settings.

#include "NormalGun.h"


// Sets default values
ANormalGun::ANormalGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANormalGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANormalGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANormalGun::FireGun()
{

}

