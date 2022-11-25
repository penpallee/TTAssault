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

	virtual void OnAxisHorizontal(float value);
	virtual void OnAxisVertical(float value);
	virtual void onAxisMouseX(float value);
	virtual void onAxisMouseY(float value);
	virtual void onActionBoost();
	virtual void OnActionFire();
	virtual void OnPlayerHit(int damage);
	virtual void Stop();
	virtual void onSelPrimary();
	virtual void onSelSecondary();
	virtual void onSelTetertiary();

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
