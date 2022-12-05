// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicWeapon.h"
#include <TimerManager.h>
#include <Engine/EngineTypes.h>

// Sets default values
ABasicWeapon::ABasicWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Remain = Ammo;
	isMagazine = false;
	isCoolDown = true;

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
	return Remain;
}

int ABasicWeapon::returnMagazine()
{
	return Ammo;;
}

FString ABasicWeapon::returnName()
{
	return myName;
}

void ABasicWeapon::MagazineReloadComplete()
{
	Remain = Ammo;
}

void ABasicWeapon::AutoFire()
{
}

void ABasicWeapon::RemainReload()
{
	Remain++;
}

void ABasicWeapon::CoolComplete()
{
	
}
