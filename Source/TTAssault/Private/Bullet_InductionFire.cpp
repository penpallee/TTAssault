// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_InductionFire.h"

#include "AssaultBoss.h"
#include "Character_Danmoozi.h"
#include "Character_Soondae.h"
#include "Components/AudioComponent.h"
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

	SetRootComponent(BulletCollisionComp);
	BulletCollisionComp->SetGenerateOverlapEvents(true);
	BulletCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BulletCollisionComp->SetCollisionProfileName(TEXT("BossBullet"));
	bulletTrailFX->bAllowRecycling = true;

	bulletMovementComp->InitialSpeed = 1200;
	bulletMovementComp->MaxSpeed = 1200;
	bulletMovementComp->ProjectileGravityScale = 0;
	bulletMovementComp->bIsHomingProjectile = true;
	bulletMovementComp->HomingAccelerationMagnitude = 1200;
}

// Called when the game starts or when spawned
void ABullet_InductionFire::BeginPlay()
{
	Super::BeginPlay();

	BulletCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet_InductionFire::OnCapsuleComponentBeginOverlap);


	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireTrailSound, GetActorLocation());
	FTimerHandle GravityTimerHandle;
	float GravityTime = 3.2;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			Destroying();
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
	if (OtherActor->IsA(ACharacter_Danmoozi::StaticClass()))
	{
		Cast<ACharacter_Danmoozi>(OtherActor)->OnPlayerHit(2);
		Destroying();
	}
	else if (OtherActor->IsA(ACharacter_Soondae::StaticClass()))
	{
		Cast<ACharacter_Soondae>(OtherActor)->OnPlayerHit(2);
		Destroying();
	}
	else
	{
		Destroying();
	}
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

void ABullet_InductionFire::Destroying()
{
	if (!explosionFX->IsLooping())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation());
		// if (audio == nullptr || false == audio->IsPlaying()) {
		// 	audio = UGameplayStatics::SpawnSound2D(GetWorld(), fireDestroySound);
		// }
	}
	this->Destroy();
}

