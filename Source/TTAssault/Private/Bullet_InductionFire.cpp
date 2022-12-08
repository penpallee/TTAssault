// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_InductionFire.h"

#include "AssaultBoss.h"
#include "Character_Danmoozi.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABullet_InductionFire::ABullet_InductionFire()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("inductionFireCollisionComp"));
	bulletTrailFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("bulletTrailFX"));
	bulletMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("bulletMovement"));

	bulletTrailFX->SetupAttachment(BulletCollisionComp);
	bulletMovementComp->SetUpdatedComponent(BulletCollisionComp);

	// SetRootComponent(inductionFireCollisionComp);
	BulletCollisionComp->SetGenerateOverlapEvents(true);
	BulletCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BulletCollisionComp->SetCollisionProfileName(TEXT("BossBullet"));
	bulletTrailFX->bAllowRecycling = true;

	bulletMovementComp->InitialSpeed = 1200;
	bulletMovementComp->MaxSpeed = 1200;
	bulletMovementComp->ProjectileGravityScale = 0;
	bulletMovementComp->bIsHomingProjectile = true;
	bulletMovementComp->HomingAccelerationMagnitude = 1200;

	BulletCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet_InductionFire::OnCapsuleComponentBeginOverlap);

}

// Called when the game starts or when spawned
void ABullet_InductionFire::BeginPlay()
{
	Super::BeginPlay();	

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireTrailSound, GetActorLocation());
	FTimerHandle GravityTimerHandle;
	float GravityTime = 2.1;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			this->Destroy();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireDestroySound, GetActorLocation());
	
			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);
	
}

// Called every frame
void ABullet_InductionFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet_InductionFire::OnCapsuleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void ABullet_InductionFire::SetInductionFireTarget(TargetSel target)
{
	
	/*if (target == TargetSel::Boss)
	{
		auto targetActor = UGameplayStatics::GetActorOfClass(GetWorld(), AAssaultBoss::StaticClass());
		
	}

	else if (target == TargetSel::Player)
	{
		//auto targetActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacter_Danmoozi::StaticClass());
		bulletMovementComp->HomingTargetComponent  = AAssaultBoss::StaticClass;
	}
	*/

	
	
}

