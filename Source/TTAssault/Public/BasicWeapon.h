// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicWeapon.generated.h"

UCLASS()
class TTASSAULT_API ABasicWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int Damage;
	UPROPERTY(EditAnywhere)
	float Cooltime;
	UPROPERTY(EditAnywhere)
	int Ammo;
	UPROPERTY(EditAnywhere)
	float reloadingTime;

	int Remain;
	bool isMagazine;
	bool isCoolDown;
	FString myName;

	virtual bool FireArm();
	virtual void OnSleep();
	virtual void OnAwake();
	virtual int returnAmmo();
	virtual FString returnName();
	virtual void SetTimerMagazineReload();
	virtual void MagazineReloadComplete();
	virtual void SetTimerRemaining();
	virtual void SetTimerCoolDown();
};
