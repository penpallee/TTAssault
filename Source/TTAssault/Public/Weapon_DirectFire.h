// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicWeapon.h"
#include "Weapon_DirectFire.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API AWeapon_DirectFire : public ABasicWeapon
{
	GENERATED_BODY()

public:
	AWeapon_DirectFire();
	void Tick(float DeltaTime) override;
	void BeginPlay();
	bool FireArm();
	void OnSleep();
	void OnAwake();
	virtual int returnAmmo();
	virtual FString returnName();

	UPROPERTY(EditAnywhere)
	USoundBase* fireSound;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AMyGrenade> bulletFactory;
};
