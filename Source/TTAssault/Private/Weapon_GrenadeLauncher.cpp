// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_GrenadeLauncher.h"
#include "MyGrenade.h"
#include <Kismet/GameplayStatics.h>

AWeapon_GrenadeLauncher::AWeapon_GrenadeLauncher()
{
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("SkeletalMesh'/Game/TH/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGun.Object);
		gunMeshComp->SetVisibility(false);
	}
}

void AWeapon_GrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();
	Damage = 50;
	Cooltime = 1.0f;
	Ammo = 10;
}

void AWeapon_GrenadeLauncher::FireArm()
{
	Super::FireArm();
	FTransform t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<AMyGrenade>(bulletFactory, t);

	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}


void AWeapon_GrenadeLauncher::OnSleep()
{
	gunMeshComp->SetVisibility(false);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
void AWeapon_GrenadeLauncher::OnAwake()
{
	gunMeshComp->SetVisibility(true);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}