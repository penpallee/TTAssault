// Fill out your copyright notice in the Description page of Project Settings.


#include "ChildAssaultCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

AChildAssaultCharacter::AChildAssaultCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));//���̷�Ż �޽� ������

	if (tempBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempBody.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetRelativeRotation(FRotator(-90, 0, 0));
}

// Called when the game starts or when spawned
void AChildAssaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	InputComponent->BindAction(TEXT("Fire"), IE_Pressed ,this, &AChildAssaultCharacter::onActionFire);
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AChildAssaultCharacter::Jump);
	InputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AChildAssaultCharacter::OnAxisHorizontal);
	InputComponent->BindAxis(TEXT("Move Right / Left"), this, &AChildAssaultCharacter::OnAxisVertical); 
	InputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &AChildAssaultCharacter::onAxisMouse);
}

// Called every frame
void AChildAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	cameraComp->SetRelativeLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, 1000));

	//this->SetActorRotation(FRotator(0, 0, 0));
	this->SetActorLocationAndRotation((GetActorLocation() + direction * 600 * DeltaTime),(GetActorRotation()+rotation*DeltaTime));
}

// Called to bind functionality to input
void AChildAssaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChildAssaultCharacter::OnAxisHorizontal(float value)
{
	direction.X = value;
}

void AChildAssaultCharacter::OnAxisVertical(float value)
{
	direction.Y = value;
}

void AChildAssaultCharacter::onAxisMouse(float value)
{
	rotation.Roll = value;
}

void AChildAssaultCharacter::onActionBoost()
{

}

void AChildAssaultCharacter::onActionFire()
{

}

void AChildAssaultCharacter::Jump()
{
	Super::Jump();
}
