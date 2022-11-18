// Fill out your copyright notice in the Description page of Project Settings.


#include "ChildAssaultCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

AChildAssaultCharacter::AChildAssaultCharacter()
{
	//Super();
}

// Called when the game starts or when spawned
void AChildAssaultCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChildAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//cameraComp->SetWorldLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, 1000));
}

// Called to bind functionality to input
void AChildAssaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChildAssaultCharacter::OnAxisHorizontal(float value)
{
	Super::OnAxisHorizontal(value);
}

void AChildAssaultCharacter::OnAxisVertical(float value)
{
	Super::OnAxisVertical(value);
}

void AChildAssaultCharacter::onActionBoost()
{
	Super::onActionBoost();
}

void AChildAssaultCharacter::onActionFire()
{
	Super::onActionFire();
}

void AChildAssaultCharacter::onAxisMouseX(float value)
{
	Super::onAxisMouseX(value);
}

void AChildAssaultCharacter::onAxisMouseY(float value)
{
	Super::onAxisMouseY(value);
}
