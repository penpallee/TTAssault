// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicWeapon.h"
#include "Weapon_GrenadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API AWeapon_GrenadeLauncher : public ABasicWeapon
{
	GENERATED_BODY()
	UPROPERTY(replicated)
		AWeapon_GrenadeLauncher* grenadeOwner;
public:
	AWeapon_GrenadeLauncher();
	void Tick(float DeltaTime) override;
	void BeginPlay();
	//UFUNCTION(NetMulticast, reliable)
		bool FireArm();
	UFUNCTION(NetMulticast, reliable)
		void OnSleep();
	UFUNCTION(NetMulticast, reliable)
		void OnAwake();
	int returnAmmo();
	FString returnName();

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet_Grenade> bulletFactory;

	void RemainReload();
	void CoolComplete();
};
