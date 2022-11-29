// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicWeapon.h"
#include "Weapon_Pipe.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API AWeapon_Pipe : public ABasicWeapon
{
	GENERATED_BODY()
public:
	AWeapon_Pipe();
	void Tick(float DeltaTime) override;
	void BeginPlay();
	bool FireArm();
	void OnSleep();
	void OnAwake();
	virtual int returnAmmo();
	virtual FString returnName();

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meleeMeshComp;
	UPROPERTY(EditAnywhere)
	USoundBase* fireSound;
};
