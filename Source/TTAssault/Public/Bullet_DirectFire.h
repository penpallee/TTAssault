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

	UPROPERTY(EditAnywhere)
	class UParticleSystem* explosionFX;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* bulletTrailFX;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* bulletMovementComp;
};
