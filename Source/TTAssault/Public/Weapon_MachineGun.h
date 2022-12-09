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
	UPROPERTY(replicated)
		AWeapon_MachineGun* machineOwner;
public:
	AWeapon_MachineGun();
	void Tick(float DeltaTime) override;
	void BeginPlay();
	//UFUNCTION(NetMulticast, reliable)
		bool FireArm();
	UFUNCTION(NetMulticast, reliable)
		void FireStop();
	UFUNCTION(NetMulticast, reliable)
		void OnSleep();
	UFUNCTION(NetMulticast, reliable)
		void OnAwake();
	int returnAmmo();
	FString returnName();
	
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet_MachineGun> bulletFactory;

	void MagazineReloadComplete() override;
	void CoolComplete() override;
	UFUNCTION(NetMulticast, reliable)
		void AutoFire() override;
};
