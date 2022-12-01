// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_DirectFire.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABullet_DirectFire::ABullet_DirectFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletTrailFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("bulletTrailFX"));
	bulletMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("bulletMovement"));

	bulletTrailFX->bAllowRecycling = true;

	bulletMovementComp->InitialSpeed = 1200;
	bulletMovementComp->MaxSpeed = 1200;
	bulletMovementComp->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void ABullet_DirectFire::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle GravityTimerHandle;
	float GravityTime = 4;
	
	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// �ڵ� ����
			this->Destroy();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation());

			// TimerHandle �ʱ�ȭ
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);	// �ݺ��Ϸ��� false�� true�� ����
}

// Called every frame
void ABullet_DirectFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

