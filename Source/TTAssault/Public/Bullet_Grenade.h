// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Particles/ParticleSystem.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include <Engine/EngineTypes.h>
#include "Bullet_Grenade.generated.h"

UCLASS()
class TTASSAULT_API ABullet_Grenade : public AActor
{
	GENERATED_BODY()
		UPROPERTY(replicated)
		ABullet_Grenade* bulletOwner;
	
public:	
	// Sets default values for this actor's properties
	ABullet_Grenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	UFUNCTION(NetMulticast, reliable)
		virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, reliable)
		void OnSphereComponentBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;
	UPROPERTY(EditAnywhere)
	USoundBase* fireSound;

	float life;
	float radius;
	float speed = 1000;

	UFUNCTION(NetMulticast, reliable)
		void Expolosion();
	/*void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);*/

	UPROPERTY(EditAnywhere)
	class UParticleSystem* explosionVFXFactory;
};
