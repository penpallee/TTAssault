// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicWeapon.h"
#include <TimerManager.h>
#include <Engine/EngineTypes.h>

// Sets default values
ABasicWeapon::ABasicWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	remain = ammo;
	isMagazine = false;
	isCoolDown = true;
	combo = 1;

	// From "custom" collision channels.
	ECollisionChannel CollisionChannel;
	FCollisionResponseParams ResponseParams;
	if (UCollisionProfile::GetChannelAndResponseParams(FName(TEXT("AimLayer")), CollisionChannel, ResponseParams))
	{
		objTypes.Add(UEngineTypes::ConvertToObjectType(CollisionChannel));
	}
}

// Called when the game starts or when spawned
void ABasicWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABasicWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABasicWeapon::FireArm()
{
	return true;
}

void ABasicWeapon::OnSleep()
{

}
void ABasicWeapon::OnAwake()
{

}

int ABasicWeapon::returnAmmo()
{
	return remain;
}

int ABasicWeapon::returnMagazine()
{
	return ammo;;
}

FString ABasicWeapon::returnName()
{
	return myName;
}

float ABasicWeapon::returnCoolTime()
{
	return coolTime;
}

float ABasicWeapon::returnCoolProgress()
{
	return coolTimeProgress;
}

float ABasicWeapon::returnReloadTime()
{
	return reloadingTime;
}

float ABasicWeapon::returnReloadProgress()
{
	return reloadingProgress;
}

void ABasicWeapon::MagazineReloadComplete()
{
	remain = ammo;
}

void ABasicWeapon::AutoFire()
{
}

void ABasicWeapon::RemainReload()
{
	remain++;
}

void ABasicWeapon::CoolComplete()
{
}

int ABasicWeapon::GetCombo()
{
	return combo;
}

void ABasicWeapon::SetCombo()
{
	combo++;
	if (combo > comboMax)
		combo = 1;
}

void ABasicWeapon::Explosion()
{
}
