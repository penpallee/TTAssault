// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicWeapon.h"
#include "Weapon_SniperRifle.generated.h"

/**
 *
 */
UCLASS()
class TTASSAULT_API AWeapon_SniperRifle : public ABasicWeapon
{
	GENERATED_BODY()
public:
	AWeapon_SniperRifle();
	void Tick(float DeltaTime) override;
	void BeginPlay();
	bool FireArm();
	void OnSleep();
	void OnAwake();
	int returnAmmo();

	UFUNCTION(BlueprintCallable)
	FString returnName();

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* sniperMeshComp;
	//UPROPERTY(EditAnywhere)
	//USoundBase* fireSound;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* bulletImpactFactory;
	//UPROPERTY(EditAnywhere)
	//USoundBase* reloadSound;

	void MagazineReloadComplete() override;
	void CoolComplete() override;
};
