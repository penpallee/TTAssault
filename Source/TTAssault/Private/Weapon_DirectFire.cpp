// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_DirectFire.h"
#include "Kismet/GameplayStatics.h"

AWeapon_DirectFire::AWeapon_DirectFire()
{
}

void AWeapon_DirectFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon_DirectFire::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle GravityTimerHandle;
	float GravityTime = 4;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			this->Destroy();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation());

			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);
}

bool AWeapon_DirectFire::FireArm()
{
	return Super::FireArm();
}

void AWeapon_DirectFire::OnSleep()
{
	Super::OnSleep();
}

void AWeapon_DirectFire::OnAwake()
{
	Super::OnAwake();
}

int AWeapon_DirectFire::returnAmmo()
{
	return Super::returnAmmo();
}

FString AWeapon_DirectFire::returnName()
{
	return Super::returnName();
}
