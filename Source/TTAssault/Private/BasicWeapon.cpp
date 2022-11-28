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

FString ABasicWeapon::returnName()
{
	return myName;
}

void ABasicWeapon::SetTimerMagazineReload()
{
	FTimerHandle T;
	GetWorldTimerManager().SetTimer(T, this, &ABasicWeapon::MagazineReloadComplete, reloadingTime, false, 0);
}

void ABasicWeapon::MagazineReloadComplete()
{
	Remain = Ammo;
}

void ABasicWeapon::SetTimerRemaining()
{
	
}

void ABasicWeapon::SetTimerCoolDown()
{

}
