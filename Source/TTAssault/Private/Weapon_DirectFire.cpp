// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_DirectFire.h"
#include "Bullet_DirectFire.h"
#include "Kismet/GameplayStatics.h"

AWeapon_DirectFire::AWeapon_DirectFire()
{
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("SkeletalMesh'/Game/TH/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGun.Object);
		gunMeshComp->SetVisibility(false);
	}
}

void AWeapon_DirectFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon_DirectFire::BeginPlay()
{
	Super::BeginPlay();

	Damage = 12;
	Cooltime = 0.5f;
	Ammo = 8;
	Remain = Ammo;
	reloadingTime = 1;
	isCoolDown = false;
	myName = TEXT("DirectFireGun");

	GetWorldTimerManager().SetTimer(autoReloadTimerHandle, this, &AWeapon_DirectFire::RemainReload, reloadingTime, true, 0);

}

bool AWeapon_DirectFire::FireArm()
{
	//return Super::FireArm();

		//Super::FireArm();
	if (Remain <= 0 || isCoolDown)
		return false;
	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_DirectFire::CoolComplete, Cooltime, false);

	FTransform t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet_DirectFire>(bulletFactory, t);

	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	Remain--;
	isCoolDown = true;

	return true;
}

void AWeapon_DirectFire::OnSleep()
{
	gunMeshComp->SetVisibility(false);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon_DirectFire::OnAwake()
{
	gunMeshComp->SetVisibility(true);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

int AWeapon_DirectFire::returnAmmo()
{
	return Remain;
}

FString AWeapon_DirectFire::returnName()
{
	return myName;
}

void AWeapon_DirectFire::RemainReload()
{
	if (Remain < Ammo)
		Remain++;
}

void AWeapon_DirectFire::CoolComplete()
{
	isCoolDown = false;
}