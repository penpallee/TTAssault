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
public:
	AWeapon_GrenadeLauncher();
	void Tick(float DeltaTime) override;
	void BeginPlay();
	void FireArm();
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
