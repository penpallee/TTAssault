// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_InductionFireArm.h"

#include "Bullet_InductionFire.h"
#include "Kismet/GameplayStatics.h"

AWeapon_InductionFireArm::AWeapon_InductionFireArm()
{
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("SkeletalMesh'/Game/TH/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGun.Object);
		gunMeshComp->SetVisibility(false);
	}
}

void AWeapon_InductionFireArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon_InductionFireArm::BeginPlay()
{
	Super::BeginPlay();
	Damage = 20;
	Cooltime = 2.0f;
	Ammo = 3;
	Remain = Ammo;
	reloadingTime = 3;
	isCoolDown = false;
	myName = TEXT("InductionFireWeapon");

	GetWorldTimerManager().SetTimer(autoReloadTimerHandle, this, &AWeapon_InductionFireArm::RemainReload, reloadingTime, true, 0);


}

bool AWeapon_InductionFireArm::FireArm(FString SelectBossORPlayer)
{
	if (Remain <= 0 || isCoolDown)
		return false;
	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_InductionFireArm::CoolComplete, Cooltime, false);

	FTransform t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet_InductionFire>(bulletFactory, t);
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	Remain--;
	isCoolDown = true;

	if (SelectBossORPlayer == FString("Boss"))	InductionBullet->SetInductionFireTarget(TargetSel::Boss);
	else if (SelectBossORPlayer == FString("Player"))	InductionBullet->SetInductionFireTarget(TargetSel::Player);

	return true;
}

void AWeapon_InductionFireArm::OnSleep()
{
	gunMeshComp->SetVisibility(false);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon_InductionFireArm::OnAwake()
{
	gunMeshComp->SetVisibility(true);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

int AWeapon_InductionFireArm::returnAmmo()
{
	return Remain;
}

FString AWeapon_InductionFireArm::returnName()
{
	return myName;
}

void AWeapon_InductionFireArm::RemainReload()
{
	if (Remain < Ammo)
		Remain++;
}

void AWeapon_InductionFireArm::CoolComplete()
{
	isCoolDown = false;
}
