// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API UCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float forwardVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rightVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isDash;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool melee;
};
