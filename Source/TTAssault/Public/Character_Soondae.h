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
	UPROPERTY(replicated)
		AAssaultCharacter* soondaeOwner;
protected:
	virtual void BeginPlay() override;
public:
	// Called to bind functionality to input
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//UFUNCTION(NetMulticast, reliable)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//UFUNCTION(NetMulticast, reliable)
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, reliable)
	void OnAxisHorizontal(float value);
	UFUNCTION(Server, reliable)
	void OnAxisHorizontalMulticast(float value);

	UFUNCTION(NetMulticast, reliable)
	void OnAxisVertical(float value);
	UFUNCTION(Server, reliable)
	void OnAxisVerticalMulticast(float value);

	UFUNCTION(NetMulticast, reliable,WithValidation)
	void onAxisMouseX(float value);
	UFUNCTION(Server, reliable)
	void onAxisMouseXMulticast(float value);

	UFUNCTION(NetMulticast, reliable, WithValidation)
	void onAxisMouseY(float value);
	UFUNCTION(Server, reliable)
	void onAxisMouseYMulticast(float value);

	UFUNCTION(NetMulticast, reliable)
	void onActionBoost();
	UFUNCTION(Server, reliable)
	void onActionBoostMulticast();

	UFUNCTION(NetMulticast, reliable)
	void OnActionFire();
	UFUNCTION(Server, reliable)
	void OnActionFireMultiCast();

	UFUNCTION(NetMulticast, reliable)
	void OnActionStop();
	UFUNCTION(Server, reliable)
	void OnActionStopMulticast();

	UFUNCTION(NetMulticast, reliable)
	void OnPlayerHit(int damage);
	UFUNCTION(Server, reliable)
	void OnPlayerHitMulticast(int damage);

	void Stop();

	UFUNCTION(NetMulticast, reliable)
	void onSelPrimary();
	UFUNCTION(Server, reliable)
	void onSelPrimaryMulticast();

	UFUNCTION(NetMulticast, reliable)
	void onSelSecondary();
	UFUNCTION(Server, reliable)
	void onSelSecondaryMulticast();

	UFUNCTION(NetMulticast, reliable)
	void onSelTetertiary();
	UFUNCTION(Server, reliable)
	void onSelTetertiaryMulticast();

	virtual void boosterCharge() override;
	UFUNCTION(BlueprintCallable)
		FPlayerStatus returnStatus();

	UPROPERTY(BlueprintReadOnly, replicated)
		class AWeapon_Pipe* pipe;
	UPROPERTY(EditAnywhere, replicated)
		TSubclassOf<class AWeapon_Pipe> pipeFactory;

	UPROPERTY(BlueprintReadOnly, replicated)
		class AWeapon_MachineGun* machineGun;
	UPROPERTY(EditAnywhere, replicated)
		TSubclassOf<class AWeapon_MachineGun> machineGunFactory;

	UPROPERTY(BlueprintReadOnly, replicated)
		class AWeapon_BeamRifle* beamRifle;
	UPROPERTY(EditAnywhere, replicated)
		TSubclassOf<class AWeapon_BeamRifle> beamRifleFactory;

	bool bTing = false;
	FVector tingDirection;
	float tingCurrentTime;

	UPROPERTY(EditAnywhere)
		float tingTime = 0.1f;
	UPROPERTY(EditAnywhere)
		float tingSpeed = 100;
};
