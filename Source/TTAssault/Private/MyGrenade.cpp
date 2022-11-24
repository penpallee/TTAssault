// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGrenade.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AMyGrenade::AMyGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	sphereComp->SetSphereRadius(1);

	sphereComp->SetGenerateOverlapEvents(true);
	sphereComp->SetCollisionProfileName(TEXT("Bullet"));
	//sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMyGrenade::OnSphereComponentBeginOverlap);
	RootComponent = sphereComp;
	meshComp->SetRelativeScale3D(FVector(0.3f));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMyGrenade::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, this, &AMyGrenade::Expolosion, 0.1f, false, 5.0f);
}

// Called every frame
void AMyGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector direction = GetActorForwardVector();
	FVector P0 = GetActorLocation();
	FVector velocity = direction * speed;

	SetActorLocation(P0 + velocity * DeltaTime, true);
}

void AMyGrenade::Expolosion()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionVFXFactory, GetActorLocation());
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld, explosionNiagaraFactory, GetActorLocation());
	this->Destroy();
}

void AMyGrenade::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Expolosion();
}