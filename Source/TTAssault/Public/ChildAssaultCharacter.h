// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssaultCharacter.h"
#include "ChildAssaultCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API AChildAssaultCharacter : public AAssaultCharacter
{
	GENERATED_BODY()
		AChildAssaultCharacter();
protected:
	virtual void BeginPlay() override;
	
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	void onAxisMouse(float value);
	void onActionBoost();
	void onActionFire();
	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void Jump();
	FVector direction;
	FRotator rotation;
};
