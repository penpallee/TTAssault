// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include "PlayerFireComponent.h"
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
	this->SetActorRotation(FRotator(0, 90, -90));

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetRelativeRotation(FRotator(0, -90, 0));
	cameraComp->SetRelativeLocation(FVector(0, 1000, 0));
	cameraComp->SetupAttachment(RootComponent);

	weaponComponent = CreateDefaultSubobject <UPlayerFireComponent>("fireComp");//¸ðµâÈ­
}

// Called when the game starts or when spawned
void AAssaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AAssaultCharacter::onActionFire);
	InputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AAssaultCharacter::onActionBoost);
	InputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AAssaultCharacter::OnAxisHorizontal);
	InputComponent->BindAxis(TEXT("Move Right / Left"), this, &AAssaultCharacter::OnAxisVertical);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &AAssaultCharacter::onAxisMouseY);
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &AAssaultCharacter::onAxisMouseX);
	speed = 500;
	booster = 1000;
}

// Called every frame
void AAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//cameraComp->SetRelativeLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, 1000));
	
	this->SetActorLocation(GetActorLocation() + direction * speed * DeltaTime);
}

// Called to bind functionality to input
void AAssaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	weaponComponent->SetupFire(PlayerInputComponent);
}

void AAssaultCharacter::OnAxisHorizontal(float value)
{
	direction.Y = -value;
}

void AAssaultCharacter::OnAxisVertical(float value)
{
	direction.X = value;
}

void AAssaultCharacter::onAxisMouseX(float value)
{

}

void AAssaultCharacter::onAxisMouseY(float value)
{
	
}

void AAssaultCharacter::onActionBoost()
{
	this->LaunchCharacter(GetActorForwardVector() * booster, false, false);
}

void AAssaultCharacter::onActionFire()
{

}

void AAssaultCharacter::OnPlayerHit(int damage)
{
	HP -= damage;
}