// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Pipe.h"
#include <Kismet/GameplayStatics.h>
#include <Components/BoxComponent.h>

#include "AssaultBoss.h"

AWeapon_Pipe::AWeapon_Pipe()
{
	meleeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meleeMeshComp"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMelee(TEXT("StaticMesh'/Game/StarterContent/Props/SM_PillarFrame.SM_PillarFrame'"));

	if (tempMelee.Succeeded())
	{
		meleeMeshComp->SetStaticMesh(tempMelee.Object);
		meleeMeshComp->SetRelativeLocation(FVector(-60, 0, 0));
		meleeMeshComp->SetRelativeScale3D(FVector(.5f));
		meleeMeshComp->SetRelativeRotation(FRotator(90, 90, 90));
	}

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(meleeMeshComp);
	boxComp->SetRelativeLocation(FVector(10, 0, 276));
	boxComp->SetRelativeScale3D(FVector(.5f));

	boxComp->SetGenerateOverlapEvents(true);
	boxComp->SetCollisionProfileName(TEXT("Bullet"));
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Pipe::OnBoxComponentBeginOverlap);
}

// Called when the game starts
void AWeapon_Pipe::BeginPlay()
{
	Super::BeginPlay();
	damage = 30;
	ammo = 2147483646;
	coolTime = 0.6f;
	comboMax = 3;
	comboTime = 0;
	remain = ammo;
	isCoolDown = false;
	myName = TEXT("TitaniumPipe");
}

void AWeapon_Pipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeapon_Pipe::FireArm()
{
	if (isCoolDown)
		return false;
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_Pipe::CoolComplete, 0.1f, true, 0);

	comboTime = 0;
	isCoolDown = true;
	return true;
}

void AWeapon_Pipe::OnSleep()
{
	meleeMeshComp->SetVisibility(false);
}
void AWeapon_Pipe::OnAwake()
{
	meleeMeshComp->SetVisibility(true);
}

int AWeapon_Pipe::returnAmmo()
{
	return remain;
}

FString AWeapon_Pipe::returnName()
{
	return myName;
}

void AWeapon_Pipe::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverLap"));

	auto hitActor = SweepResult.GetActor();
	if (OtherActor->IsA(AAssaultBoss::StaticClass()))
		Cast<AAssaultBoss>(OtherActor)->OnBossHit(damage);

	if (hitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *hitActor->GetName());
	}

	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	Explosion();
}

void AWeapon_Pipe::CoolComplete()
{
	coolTimeProgress += 0.1f;
	if (coolTimeProgress >= coolTime)
	{
		isCoolDown = false;
		GetWorldTimerManager().ClearTimer(autoFireTimerHandle);
		boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		coolTimeProgress = 0;
	}
}

void AWeapon_Pipe::Explosion()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionVFXFactory, boxComp->GetComponentLocation());
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}