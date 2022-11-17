// Fill out your copyright notice in the Description page of Project Settings.


#include "ChildAssaultCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

AChildAssaultCharacter::AChildAssaultCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));//½ºÄÌ·¹Å» ¸Þ½Ã °¡Á®¿È

	if (tempBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempBody.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 0, 0));
	}
	this->SetActorRotation(FRotator(0, 90, -90));

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetRelativeRotation(FRotator(0, -90, 0));
	cameraComp->SetRelativeLocation(FVector(0,1000,0));
	cameraComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AChildAssaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AChildAssaultCharacter::onActionFire);
	InputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AChildAssaultCharacter::onActionBoost);
	InputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AChildAssaultCharacter::OnAxisHorizontal);
	InputComponent->BindAxis(TEXT("Move Right / Left"), this, &AChildAssaultCharacter::OnAxisVertical);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &AChildAssaultCharacter::onAxisMouseY);
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &AChildAssaultCharacter::onAxisMouseX);
	speed = 500;
	booster = 1000;
}

// Called every frame
void AChildAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	cameraComp->SetWorldLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, 1000));

	this->SetActorLocation(GetActorLocation() + direction * speed * DeltaTime);

	/*direction = FTransform(GetControlRotation()).TransformVector(FVector(direction.Y,0,direction.Z));
	AddMovementInput(direction);
	direction = FVector::ZeroVector;*/

	//this->SetActorLocationAndRotation((GetActorLocation()+direction*speed*DeltaTime),(GetActorRotation()+rotation*rotRate*DeltaTime));
}

// Called to bind functionality to input
void AChildAssaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChildAssaultCharacter::OnAxisHorizontal(float value)
{
	direction.Y = value;
}

void AChildAssaultCharacter::OnAxisVertical(float value)
{
	direction.X = value;
}

void AChildAssaultCharacter::onActionBoost()
{
	this->LaunchCharacter(GetActorForwardVector() * booster, false, false);
}

void AChildAssaultCharacter::onActionFire()
{

}

void AChildAssaultCharacter::onAxisMouseX(float value)
{
}

void AChildAssaultCharacter::onAxisMouseY(float value)
{
}
