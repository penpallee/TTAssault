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
	int returnAmmo();
	FString returnName();

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meleeMeshComp;
	UPROPERTY(EditAnywhere)
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UFUNCTION()
		void OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CoolComplete();
	void Explosion(FVector ImpactPoint) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* explosionVFXFactory;
};
