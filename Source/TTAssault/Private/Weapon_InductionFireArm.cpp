// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_InductionFireArm.h"

#include "Kismet/GameplayStatics.h"

AWeapon_InductionFireArm::AWeapon_InductionFireArm()
{
}

void AWeapon_InductionFireArm::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon_InductionFireArm::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle GravityTimerHandle;
	float GravityTime = 2.1;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			this->Destroy();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation());

			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);
}

bool AWeapon_InductionFireArm::FireArm()
{
	return Super::FireArm();
}

void AWeapon_InductionFireArm::OnSleep()
{
	Super::OnSleep();
}

void AWeapon_InductionFireArm::OnAwake()
{
	Super::OnAwake();
}

int AWeapon_InductionFireArm::returnAmmo()
{
	return Super::returnAmmo();
}

FString AWeapon_InductionFireArm::returnName()
{
	return Super::returnName();
}
