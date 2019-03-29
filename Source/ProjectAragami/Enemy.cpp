// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "PlayerCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Components
	collisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("collisionComponent"));
	RootComponent = collisionComponent;
	collisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	collisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	collisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnBeginOverlap);

	//AI Module
	aiPercComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("aiPercComp"));
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("sightConfig"));
	sightConfig->SightRadius = 300.0f;
	sightConfig->LoseSightRadius = 320.0f;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->SetMaxAge(0.1f);

	aiPercComp->ConfigureSense(*sightConfig);
	aiPercComp->SetDominantSense(sightConfig->GetSenseImplementation());
	aiPercComp->OnPerceptionUpdated.AddDynamic(this, &AEnemy::OnSensed);

	currVel = FVector::ZeroVector;
	speed = 30.0f;
	maxHealth = 30.0f;
	currentHealth = maxHealth;
	distSq = BIG_NUMBER;
	backToBase = false;


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	baseLoc = this->GetActorLocation();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 1000.0f, FColor::Red, true);

	// Handle movement based on where Enemy is to baseLoc
	if (!currVel.IsZero())
	{
		FVector newLocation = GetActorLocation() + currVel * DeltaTime;
		if (backToBase)
		{
			//UE_LOG(LogTemp, Warning, TEXT("dsq= %f"), distSq);
			if ((newLocation - baseLoc).SizeSquared2D() < distSq)
			{
				distSq = (newLocation - baseLoc).SizeSquared2D();
			}
			else
			{	//if dist bigger, stop
				currVel = FVector::ZeroVector;
				distSq = BIG_NUMBER;
				backToBase = false;
				setNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}
		SetActorLocation(newLocation);
	}
}

void AEnemy::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	if (OtherActor != this)
	{
		APlayerCharacter* p = Cast<APlayerCharacter>(OtherActor);
		if (p)
		{
			//p->health -= 10;
		}
	}
}

void AEnemy::OnSensed(const TArray<AActor*>& updatedActors)
{
	for (int i = 0; i < updatedActors.Num(); i++)
	{
		FString n = updatedActors[i]->GetName();
		FActorPerceptionBlueprintInfo info;
		aiPercComp->GetActorsPerception(updatedActors[i], info);
		if (info.LastSensedStimuli[0].WasSuccessfullySensed()) 
		{
			FVector dir = updatedActors[i]->GetActorLocation() - GetActorLocation();
			dir.Z = 0.0f;	//reset as they may be at diff Z values
			DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + dir * 1000.0f, FColor::Red, true);
			currVel = dir.GetSafeNormal() * speed;	//set vel towards actor
			setNewRotation(updatedActors[i]->GetActorLocation(), GetActorLocation());
		}
		else 
		{	//actor has moved away from radius
			FVector dir = baseLoc - GetActorLocation();
			dir.Z = 0.0f;	//reset as they may be at diff Z values
			if (dir.SizeSquared2D() > 1.0f) 
			{	//set vel towards baseLoc
				currVel = dir.GetSafeNormal() * speed;
				backToBase = true;
				setNewRotation(baseLoc, GetActorLocation());
			}
		}
	}
}

void AEnemy::setNewRotation(FVector targPos, FVector currPos)
{
	FVector newD = targPos - currPos;	//diff in curr loc and Enemy loc
	newD.Z = 0.0f;	//we are NOT interested in any Z diff
	FRotator rotDir = newD.Rotation() - GetActorForwardVector().Rotation(); // diff in reqd dir & curr. dir
	enemyRot = newD.Rotation();
	SetActorRotation(enemyRot);
}

