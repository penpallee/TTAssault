// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_Grenade.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>
#include "AssaultBoss.h"
#include "Character_Danmoozi.h"

// Sets default values
ABullet_Grenade::ABullet_Grenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	sphereComp->SetSphereRadius(1);

	/*sphereComp->SetGenerateOverlapEvents(true);
	sphereComp->SetCollisionProfileName(TEXT("Bullet"));
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet_Grenade::OnSphereComponentBeginOverlap);*/
	RootComponent = sphereComp;
	meshComp->SetRelativeScale3D(FVector(0.2f));
	meshComp->SetupAttachment(RootComponent);

	sphereComp->SetGenerateOverlapEvents(true);
	sphereComp->SetNotifyRigidBodyCollision(true);
	sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	sphereComp->SetCollisionProfileName(TEXT("Bullet"));
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet_Grenade::OnSphereComponentBeginOverlap);

	bReplicates = true;
}

void ABullet_Grenade::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABullet_Grenade, bulletOwner);
}

// Called when the game starts or when spawned
void ABullet_Grenade::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &ABullet_Grenade::Expolosion, 5, false);
}

// Called every frame
void ABullet_Grenade::Tick_Implementation(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector direction = GetActorForwardVector();
	FVector P0 = GetActorLocation();
	FVector velocity = direction * speed;

	SetActorLocation(P0 + velocity * DeltaTime, true);
}

void ABullet_Grenade::Expolosion_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionVFXFactory, GetActorLocation());
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	this->Destroy();
}

void ABullet_Grenade::OnSphereComponentBeginOverlap_Implementation(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AAssaultBoss::StaticClass()))
	{
		Cast<AAssaultBoss>(OtherActor)->OnBossHit(10);
		Expolosion_Implementation();
	}
	if (!OtherActor->IsA(ACharacter_Danmoozi::StaticClass()))
		Expolosion_Implementation();
}