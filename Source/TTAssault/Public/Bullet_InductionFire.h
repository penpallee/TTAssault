// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet_InductionFire.generated.h"

UENUM(BlueprintType)
enum class TargetSel : uint8 {
	Boss UMETA(displayname = BOSS),
	Player UMETA(displayname = PLAYER),
};

UCLASS()
class TTASSAULT_API ABullet_InductionFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet_InductionFire();


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

	void SetInductionFireTarget(TargetSel target);
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* explosionFX;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* bulletTrailFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* bulletMovementComp;

	UPROPERTY(EditAnywhere)
	TargetSel SetTarget;

	UPROPERTY(EditAnywhere)
	class USoundBase* fireTrailSound;

	UPROPERTY(EditAnywhere)
	class USoundBase* fireDestroySound;

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* inductionFireCollisionComp;
};
