// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet_MachineGun.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ABullet_MachineGun::ABullet_MachineGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	sphereComp->SetSphereRadius(1);

	sphereComp->SetGenerateOverlapEvents(true);
	sphereComp->SetCollisionProfileName(TEXT("Bullet"));
	//sphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet_MachineGun::OnSphereComponentBeginOverlap);
	RootComponent = sphereComp;
	meshComp->SetRelativeScale3D(FVector(0.1f, 0.05f, 0.05f));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABullet_MachineGun::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &ABullet_MachineGun::Expolosion, 0.1f, false, 5.0f);
}

// Called every frame
void ABullet_MachineGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector direction = GetActorForwardVector();
	FVector P0 = GetActorLocation();
	FVector velocity = direction * speed;

	SetActorLocation(P0 + velocity * DeltaTime, true);
}

void ABullet_MachineGun::Expolosion()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionVFXFactory, GetActorLocation());
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld, explosionNiagaraFactory, GetActorLocation());
	this->Destroy();
}

void ABullet_MachineGun::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Expolosion();
}