// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet_DirectFire.generated.h"

UCLASS()
class TTASSAULT_API ABullet_DirectFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet_DirectFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCapsuleComponentBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void Destroying();

	UPROPERTY(EditAnywhere)
	class UParticleSystem* explosionFX;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* bulletTrailFX;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* bulletMovementComp;

	
	UPROPERTY(EditAnywhere)
	class USoundBase* fireTrailSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* fireDestroySound;

	UPROPERTY(EditAnywhere)
	class USphereComponent* BulletCollisionComp;;

	// UPROPERTY(EditAnywhere)
	// TArray<class UParticleSystem> RandomFireFX;
};
