// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

// Sets default values
AAssaultCharacter::AAssaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));//½ºÄÌ·¹Å» ¸Þ½Ã °¡Á®¿È

	if (tempBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempBody.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetRelativeRotation(FRotator(-90, 0, 0));
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
}

// Called to bind functionality to input
void AAssaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAssaultCharacter::onActionBoost()
{

}

void AAssaultCharacter::onActionFire()
{

}
