// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_SniperRifle.h"
#include <Kismet/GameplayStatics.h>

AWeapon_SniperRifle::AWeapon_SniperRifle()
{
	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperMeshComp"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/TH/Resources/SniperGun/Sniper.Sniper'"));

	if (tempSniper.Succeeded())
	{
		sniperMeshComp->SetStaticMesh(tempSniper.Object);
		sniperMeshComp->SetRelativeScale3D(FVector(.15f));
		sniperMeshComp->SetVisibility(false);
	}
}

void AWeapon_SniperRifle::BeginPlay()
{
	Super::BeginPlay();
	Damage = 20;
	Cooltime = 20.0f;
	Ammo = 20;
}

void AWeapon_SniperRifle::FireArm()
{
	FVector start = sniperMeshComp->GetSocketLocation(TEXT("FirePosition"));
	FVector end = start + sniperMeshComp->GetRightVector() * 300000.0f;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params))
	{
		FTransform impactTransform(hitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, impactTransform);

		auto hitComp = hitInfo.GetComponent();
		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			FVector dir = (hitInfo.ImpactPoint - start).GetSafeNormal();
			FVector force = dir * hitComp->GetMass() * 500000;

			hitInfo.Component->AddForceAtLocation(force, dir);
		}
	}
}