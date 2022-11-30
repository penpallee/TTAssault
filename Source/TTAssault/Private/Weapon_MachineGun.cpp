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
	Damage = 2;
	Cooltime = 0.03f;
	Ammo = 30;
	Remain = Ammo;
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
	if (Remain <= 0 || isCoolDown)
		return false;

	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_MachineGun::AutoFire, Cooltime, true, 0);

	return true;
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
	return Remain;
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
	UE_LOG(LogTemp, Warning, TEXT("MagazineComplete"));
	Remain = 5;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), reloadSound, this->GetActorLocation(), 1, 1, 0);
	//GetWorldTimerManager().ClearTimer(autoReloadTimerHandle);
}

void AWeapon_MachineGun::AutoFire()
{
	FTransform t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet_MachineGun>(bulletFactory, t);
	//PlayAttackAnim();
}