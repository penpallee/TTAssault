// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_DirectFire.h"

#include "Character_Danmoozi.h"
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

	BulletCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet_DirectFire::OnCapsuleComponentBeginOverlap);
	
	
}

// Called when the game starts or when spawned
void ABullet_DirectFire::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireTrailSound, GetActorLocation());

	FTimerHandle GravityTimerHandle;
	float GravityTime = 4;
	
	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			this->Destroy();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireDestroySound, GetActorLocation());

			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);	// 반복하려면 false를 true로 변경
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
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("123112312312312312323"));
		Cast<ACharacter_Danmoozi>(OtherActor)->OnPlayerHit(5);
		this->Destroy();
		
	}
}

