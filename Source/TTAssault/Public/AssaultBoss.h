// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AssaultBoss.generated.h"

UCLASS()
class TTASSAULT_API AAssaultBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAssaultBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnCapsuleComponentHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	// UFUNCTION()
	// void OnCapsuleComponentBeginOverlap(
	// 	UPrimitiveComponent* OverlappedComponent,
	// 	AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,
	// 	bool bFromSweep,const FHitResult& SweepResult);
	
public:

	UFUNCTION(BlueprintCallable)
	void OnBossHit(int damage);

	UFUNCTION(BlueprintCallable)
	void OnBossStunned();

	UPROPERTY(EditAnywhere)
	float booster = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxHP = HP;
	UPROPERTY(EditAnywhere)
	bool isBooster = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Defense;

	void OnAxisVertical(float value);
	void OnAxisHorizontal(float value);
	void OnAxisHorizontalView(float value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* particleComp1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystemComponent* particleComp2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* StunnedVFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UProjectileMovementComponent* BossMoveComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* BossBodyAttackFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UParticleSystem* DestroyingFX;

	UPROPERTY(EditAnywhere)
	class USoundBase* DestroyingSound;
	
};
