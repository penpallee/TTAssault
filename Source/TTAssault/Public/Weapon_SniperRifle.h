// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicWeapon.h"
#include "Weapon_SniperRifle.generated.h"

UCLASS()
class TTASSAULT_API AWeapon_SniperRifle : public ABasicWeapon
{
	GENERATED_BODY()
		UPROPERTY(replicated)
		AWeapon_SniperRifle* sniperOwner;
public:
	AWeapon_SniperRifle();
	void Tick(float DeltaTime) override;
	void BeginPlay();
	//UFUNCTION(NetMulticast, reliable)
		bool FireArm();
	UFUNCTION(NetMulticast, reliable)
		void OnSleep();
	UFUNCTION(NetMulticast, reliable)
		void OnAwake();
	int returnAmmo();

	UFUNCTION(BlueprintCallable)
	FString returnName();

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* sniperMeshComp;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* bulletImpactFactory;

	virtual void MagazineReloadComplete() override;
	virtual void CoolComplete() override;
	UFUNCTION(NetMulticast, reliable)
		virtual void Explosion(FVector ImpactPoint) override;
};
