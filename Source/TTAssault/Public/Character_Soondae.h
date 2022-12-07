// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AssaultCharacter.h"
#include "Character_Soondae.generated.h"

UCLASS()
class TTASSAULT_API ACharacter_Soondae : public AAssaultCharacter
{
	GENERATED_BODY()
		ACharacter_Soondae();
protected:
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void onAxisMouseX(float value);
	void onAxisMouseY(float value);
	void onActionBoost();
	void OnActionFire();
	void OnActionStop();
	void OnPlayerHit(int damage);
	void Stop();
	void onSelPrimary();
	void onSelSecondary();
	void onSelTetertiary();
	virtual void boosterCharge() override;
	UFUNCTION(BlueprintCallable)
		FPlayerStatus returnStatus();

	UPROPERTY(BlueprintReadOnly)
		class AWeapon_Pipe* pipe;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon_Pipe> pipeFactory;

	UPROPERTY(BlueprintReadOnly)
		class AWeapon_MachineGun* machineGun;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon_MachineGun> machineGunFactory;

	UPROPERTY(BlueprintReadOnly)
		class AWeapon_BeamRifle* beamRifle;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon_BeamRifle> beamRifleFactory;

	bool bTing = false;
	FVector tingDirection;
	float tingCurrentTime;

	UPROPERTY(EditAnywhere)
		float tingTime = 0.1f;
	UPROPERTY(EditAnywhere)
		float tingSpeed = 100;
};
