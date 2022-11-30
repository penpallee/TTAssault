// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicWeapon.h"
#include "Weapon_MachineGun.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API AWeapon_MachineGun : public ABasicWeapon
{
	GENERATED_BODY()
public:
	AWeapon_MachineGun();
	void Tick(float DeltaTime) override;
	void BeginPlay();
	bool FireArm();
	void OnSleep();
	void OnAwake();
	int returnAmmo();
	FString returnName();

	//UPROPERTY(EditAnywhere)
	//USoundBase* fireSound;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet_MachineGun> bulletFactory;

	void MagazineReloadComplete() override;
	void CoolComplete() override;
	void AutoFire() override;
};
