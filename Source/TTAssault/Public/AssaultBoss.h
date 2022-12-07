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

	// UFUNCTION()
	// void OnCapsuleComponentBeginOverlap(
	// 	UPrimitiveComponent* OverlappedComponent,
	// 	AActor* OtherActor,
	// 	UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,
	// 	bool bFromSweep,const FHitResult& SweepResult);
	//
public:

	UFUNCTION(BlueprintCallable)
	void OnBossHit(int damage);

	UPROPERTY(EditAnywhere)
	float booster = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP = 100;
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
	class UProjectileMovementComponent* BossMoveComp;
	

	
};
