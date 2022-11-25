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

	void OnPlayerHit(int damage);
	void Stop();
};
