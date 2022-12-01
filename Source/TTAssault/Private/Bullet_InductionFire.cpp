// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_InductionFire.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABullet_InductionFire::ABullet_InductionFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletTrailFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("bulletTrailFX"));
	bulletMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("bulletMovement"));

	bulletTrailFX->bAllowRecycling = true;

	bulletMovementComp->InitialSpeed = 800;
	bulletMovementComp->MaxSpeed = 800;
	bulletMovementComp->ProjectileGravityScale = 0;
	bulletMovementComp->bIsHomingProjectile = true;
	bulletMovementComp->HomingAccelerationMagnitude = 800;

	

}

// Called when the game starts or when spawned
void ABullet_InductionFire::BeginPlay()
{
	Super::BeginPlay();	

	FTimerHandle GravityTimerHandle;
	float GravityTime = 2.1;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			this->Destroy();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation());

			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);
	
}

// Called every frame
void ABullet_InductionFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

