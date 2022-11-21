// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include "PlayerFireComponent.h"
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AAssaultCharacter::AAssaultCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn'"));//½ºÄÌ·¹Å» ¸Þ½Ã °¡Á®¿È
	if (tempBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempBody.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	}

	this->SetActorRotation(FRotator(0, 0, -90));

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	springArmComp->SetupAttachment((RootComponent));
	springArmComp->SetRelativeLocation(FVector(0, -90, 0));
	springArmComp->TargetArmLength = 1000;

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArmComp);

	weaponComponent = CreateDefaultSubobject <UPlayerFireComponent>("fireComp");//¸ðµâÈ­

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());

	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperMeshComp"));
	sniperMeshComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("SkeletalMesh'/Game/TH/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/TH/Resources/SniperGun/Sniper.Sniper'"));

	if (tempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGun.Object);
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	if (tempSniper.Succeeded())
	{
		sniperMeshComp->SetStaticMesh(tempSniper.Object);
		sniperMeshComp->SetRelativeScale3D(FVector(.15f));
		sniperMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}
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
	InputComponent->BindAction(TEXT("WeaponPrimary"), IE_Pressed, this, &AAssaultCharacter::onSelPrimary);
	InputComponent->BindAction(TEXT("WeaponSecondary"), IE_Pressed, this, &AAssaultCharacter::onSelSecondary);
	InputComponent->BindAction(TEXT("WeaponTertiary"), IE_Pressed, this, &AAssaultCharacter::onSelTeTertiary);
	speed = 500;
	booster = 1000;
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
}

// Called every frame
void AAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

}

void AAssaultCharacter::onActionFire()
{
	
}

void AAssaultCharacter::OnPlayerHit(int damage)
{
	HP -= (damage- Defense);
	if (HP <= 0)
	{
		Destroy();
	}
}

void AAssaultCharacter::Stop()
{
	
}

void AAssaultCharacter::onSelPrimary()
{
	SetGun(PRIMARY);
}

void AAssaultCharacter::onSelSecondary()
{
	SetGun(SECONDARY);
}

void AAssaultCharacter::onSelTeTertiary()
{
	SetGun(TERTIARY);
}

void AAssaultCharacter::SetGun(int num)
{
	switch (num)
	{
	case PRIMARY:
		gunMeshComp->SetVisibility(true);
		sniperMeshComp->SetVisibility(false);
		//nowWeapon = Cast<UPrimitiveComponent>(gunMeshComp);
		break;
	case SECONDARY:
		gunMeshComp->SetVisibility(false);
		sniperMeshComp->SetVisibility(true);
		//nowWeapon = Cast<UPrimitiveComponent>(sniperMeshComp);
		break;
	case TERTIARY:
		gunMeshComp->SetVisibility(false);
		sniperMeshComp->SetVisibility(false);
		break;
	}
}
