// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

// Sets default values
AAssaultCharacter::AAssaultCharacter()
{
}

// Called when the game starts or when spawned
void AAssaultCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	cameraComp->SetRelativeLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, 1000));
	
	//this->SetActorRotation(FRotator(0, 0, 0));
	this->SetActorLocation(GetActorLocation() + direction * 600 + DeltaTime);
}

// Called to bind functionality to input
void AAssaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAssaultCharacter::OnAxisHorizontal(float value)
{
}

void AAssaultCharacter::OnAxisVertical(float value)
{
}

void AAssaultCharacter::onAxisMouseX(float value)
{

}

void AAssaultCharacter::onAxisMouseY(float value)
{

}

void AAssaultCharacter::onActionBoost()
{

}

void AAssaultCharacter::onActionFire()
{

}
