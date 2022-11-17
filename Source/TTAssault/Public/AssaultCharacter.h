// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
#include "AssaultCharacter.generated.h"


UCLASS()
class TTASSAULT_API AAssaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAssaultCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* cameraComp;

	virtual void OnAxisHorizontal(float value); 
	virtual void OnAxisVertical(float value);
	virtual void onAxisMouseX(float value);
	virtual void onAxisMouseY(float value);
	virtual void onActionBoost();
	virtual void onActionFire();

	FVector direction;
	FRotator rotation;

	UPROPERTY(EditAnywhere)
	float speed;
	UPROPERTY(EditAnywhere)
	float booster;
	UPROPERTY(EditAnywhere)
	int HP=100;

	UPROPERTY(EditAnywhere)
	class UPlayerFireComponent* weaponComponent;

	void OnPlayerHit(int damage);
};
