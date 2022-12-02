// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicWeapon.h"
#include "Weapon_InductionFireArm.generated.h"


/**
 * 
 */
UCLASS()
class TTASSAULT_API AWeapon_InductionFireArm : public ABasicWeapon
{
	GENERATED_BODY()

public:
	AWeapon_InductionFireArm();
	void Tick(float DeltaTime) override;
	void BeginPlay();
	bool FireArm(FString SelectBossORPlayer);
	void OnSleep();
	void OnAwake();
	virtual int returnAmmo();
	virtual FString returnName();

	//UPROPERTY(EditAnywhere)
	//USoundBase* fireSound;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* explosionFX;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet_InductionFire> bulletFactory;

	class ABullet_InductionFire* InductionBullet;

	void RemainReload();
	void CoolComplete();


};
