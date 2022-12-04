// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnim.h"
#include "AssaultCharacter.h"

void UCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	auto owner = TryGetPawnOwner();
	AAssaultCharacter* player = Cast<AAssaultCharacter>(owner);
	if (player)
	{
		auto velocity = player->GetVelocity();
		auto right = player->GetActorForwardVector();
		auto forward = player->GetActorRightVector();

		forwardVelocity = FVector::DotProduct(velocity, forward);
		rightVelocity = FVector::DotProduct(velocity, right);

		UCharacterMovementComponent* movement = player->GetCharacterMovement();

		if (player->speed < 1000)
			isDash = false;
		else
			isDash = true; 
	}
}