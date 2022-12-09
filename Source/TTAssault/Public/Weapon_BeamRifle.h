// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicWeapon.h"
#include "Weapon_BeamRifle.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API AWeapon_BeamRifle : public ABasicWeapon
{
	GENERATED_BODY()
		UPROPERTY(replicated)
		AWeapon_BeamRifle* beamOwner;
	public:
	AWeapon_BeamRifle();
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
	class UStaticMeshComponent* sniperMeshComp;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet_BeamRifle> bulletFactory;

	void RemainReload();
	void CoolComplete();
};
