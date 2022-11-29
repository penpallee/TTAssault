// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssaultCharacter.h"
#include "GameFramework/Pawn.h"
#include "Character_Danmoozi.generated.h"

UCLASS()
class TTASSAULT_API ACharacter_Danmoozi : public AAssaultCharacter
{
	GENERATED_BODY()
		ACharacter_Danmoozi();
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
	void OnPlayerHit(int damage);
	void Stop();
	void onSelPrimary();
	void onSelSecondary();
	void onSelTetertiary();
	FPlayerStatus returnStatus();

	UPROPERTY(BlueprintReadOnly)
	class AWeapon_Pipe* pipe;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon_Pipe> pipeFactory;

	UPROPERTY(BlueprintReadOnly)
	class AWeapon_GrenadeLauncher* launcher;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon_GrenadeLauncher> launcherFactory;

	UPROPERTY(BlueprintReadOnly)
	class AWeapon_SniperRifle* rifle;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon_SniperRifle> rifleFactory;
};
