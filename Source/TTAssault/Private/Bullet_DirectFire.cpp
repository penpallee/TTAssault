// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_DirectFire.h"

#include "Character_Danmoozi.h"
#include "Character_Soondae.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABullet_DirectFire::ABullet_DirectFire()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("directionFireCollisionComp"));
	bulletTrailFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("bulletTrailFX"));
	bulletMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("bulletMovement"));

	bulletTrailFX->SetupAttachment(BulletCollisionComp);
	bulletMovementComp->SetUpdatedComponent(BulletCollisionComp);

	// SetRootComponent(directionFireCollisionComp);
	BulletCollisionComp->SetGenerateOverlapEvents(true);
	BulletCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BulletCollisionComp->SetCollisionProfileName(TEXT("BossBullet"));
	bulletTrailFX->bAllowRecycling = true;

	bulletMovementComp->InitialSpeed = 1200;
	bulletMovementComp->MaxSpeed = 1200;
	bulletMovementComp->ProjectileGravityScale = 0;


}

// Called when the game starts or when spawned
void ABullet_DirectFire::BeginPlay()
{
	Super::BeginPlay();

	BulletCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet_DirectFire::OnCapsuleComponentBeginOverlap);

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireTrailSound, GetActorLocation());

	FTimerHandle GravityTimerHandle;
	float GravityTime = 4;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// �ڵ� ����
			Destroying();

			// TimerHandle �ʱ�ȭ
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);	// �ݺ��Ϸ��� false�� true�� ����
}

// Called every frame
void ABullet_DirectFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet_DirectFire::OnCapsuleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacter_Danmoozi::StaticClass()))
	{
		Cast<ACharacter_Danmoozi>(OtherActor)->OnPlayerHit(8);
		Destroying();
	}
	else if (OtherActor->IsA(ACharacter_Soondae::StaticClass()))
	{
		Cast<ACharacter_Soondae>(OtherActor)->OnPlayerHit(8);
		Destroying();
	}
	else
	{
		Destroying();
	}
}

void ABullet_DirectFire::Destroying()
{
	if (IsValid(explosionFX))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation());
		if (audio == nullptr || false == audio->IsPlaying()) {
			audio = UGameplayStatics::SpawnSound2D(GetWorld(), fireDestroySound);
		}
	}
	this->Destroy();
}

