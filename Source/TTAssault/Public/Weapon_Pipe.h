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
		UPROPERTY(replicated)
		AWeapon_Pipe* pipeOwner;
public:
	AWeapon_Pipe();
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meleeMeshComp;
	UPROPERTY(EditAnywhere)
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UFUNCTION(NetMulticast, reliable)
		void OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CoolComplete();
	UFUNCTION(NetMulticast, reliable)
		void Explosion(FVector ImpactPoint) override;
	UFUNCTION(NetMulticast, reliable)
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere)
		class UParticleSystem* explosionVFXFactory;
};
