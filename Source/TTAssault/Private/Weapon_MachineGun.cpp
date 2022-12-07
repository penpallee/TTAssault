// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_MachineGun.h"
#include "Bullet_MachineGun.h"
#include <Kismet/GameplayStatics.h>

AWeapon_MachineGun::AWeapon_MachineGun()
{
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("SkeletalMesh'/Game/TH/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGun.Object);
		gunMeshComp->SetVisibility(false);
	}
	//this->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
}

void AWeapon_MachineGun::BeginPlay()
{
	Super::BeginPlay();
	damage = 2;
	coolTime = 0.03f;
	ammo = 30;
	remain = ammo;
	reloadingTime = 5;
	isCoolDown = false;
	myName = TEXT("MachineGun");
}

void AWeapon_MachineGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeapon_MachineGun::FireArm()
{
	//Super::FireArm();
	if (isCoolDown || remain <= 0)
		return false;

	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_MachineGun::AutoFire, coolTime, true, 0);

	return true;
}

void AWeapon_MachineGun::FireStop()
{
	GetWorldTimerManager().ClearTimer(autoFireTimerHandle);
}

void AWeapon_MachineGun::OnSleep()
{
	gunMeshComp->SetVisibility(false);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon_MachineGun::OnAwake()
{
	gunMeshComp->SetVisibility(true);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

int AWeapon_MachineGun::returnAmmo()
{
	return remain;
}

FString AWeapon_MachineGun::returnName()
{
	return myName;
}

void AWeapon_MachineGun::CoolComplete()
{
	isCoolDown = false;
}

void AWeapon_MachineGun::MagazineReloadComplete()
{
	reloadingProgress += 0.1f;
	if (reloadingProgress >= reloadingTime)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), reloadSound);
		remain = ammo;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), reloadSound, this->GetActorLocation(), 1, 1, 0);
		GetWorldTimerManager().ClearTimer(autoReloadTimerHandle);
		reloadingProgress = 0;
	}
}

void AWeapon_MachineGun::AutoFire()
{
	if (remain < 0)
	{
		GetWorldTimerManager().SetTimer(autoReloadTimerHandle, this, &AWeapon_MachineGun::MagazineReloadComplete, 0.1f, true, 0);
		return;
	}
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);

	FTransform t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet_MachineGun>(bulletFactory, t);
	remain--;
	//PlayAttackAnim();
}