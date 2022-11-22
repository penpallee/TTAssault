// Fill out your copyright notice in the Description page of Project Settings.


#include "Grenade.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>

// Sets default values
AGrenade::AGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	sphereComp->SetSphereRadius(64);

	sphereComp->SetGenerateOverlapEvents(true);
	sphereComp->SetCollisionProfileName(TEXT("Bullet"));
	//sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGrenade::OnSphereComponentBeginOverlap);
	RootComponent = sphereComp;
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &AGrenade::Expolosion, 5, false, 0);
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector direction = GetActorForwardVector();
	FVector P0 = GetActorLocation();
	FVector velocity = direction * speed;

	SetActorLocation(P0 + velocity * DeltaTime, true);
}

void AGrenade::Expolosion()
{
	this->Destroy();
}

void AGrenade::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//AMyEnemy* enemy = Cast<AMyEnemy>(OtherActor);
	//if(IsA(AMyEnemy::StaticClass()))
	//{
	//	OtherActor->Destroy();
	this->Destroy();
	//}
}