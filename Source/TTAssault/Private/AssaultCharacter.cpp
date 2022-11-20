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
	//ConstructorHelpers::FObjectFinder<UStaticMeshComponent> tempSniper(TEXT("StaticMesh'/Game/TH/Resources/SniperGun/Sniper.Sniper'"));
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
	booster = 10;
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;
}

// Called every frame
void AAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//cameraComp->SetRelativeLocation(FVector(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y, 1000));
	//booster -= 1;
	this->SetActorLocation(GetActorLocation() + direction * speed * DeltaTime);

	//nowWeapon->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()).X, UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()).Y, GetActorLocation().Z)));
	//gunMeshComp->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),FVector(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()).X,			UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()).Y,GetActorLocation().Z)));
	//sniperMeshComp->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()).X, UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()).Y, GetActorLocation().Z)));
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
	//GetMesh()->AddImpulse(GetActorForwardVector()*-100,TEXT("Mesh"), true);
	if (!isBooster)
	{
		booster = 10;
		//this->LaunchCharacter(direction * booster, false, false);
		//GetWorldTimerManager().SetTimer(boostHandle, this, &AAssaultCharacter::isOnBooster, 0.1f, false);
		//UCharacterMovementComponent::AddImpulse(direction,true);
	}
	isBooster = false;
}

void AAssaultCharacter::onActionFire()
{
	
}

void AAssaultCharacter::OnPlayerHit(int damage)
{
	HP -= damage;
	if (HP <= 0)
	{
		Destroy();
	}
}

void AAssaultCharacter::isOnBooster()
{
	if (isBooster)
		isBooster = false;

	GetWorldTimerManager().ClearTimer(boostHandle);
}

void AAssaultCharacter::onSelPrimary()
{
	SetGun(1);
}

void AAssaultCharacter::onSelSecondary()
{
	SetGun(2);
}

void AAssaultCharacter::onSelTeTertiary()
{
	SetGun(3);
}

void AAssaultCharacter::SetGun(int num)
{
	switch (num)
	{
	case 1:
		gunMeshComp->SetVisibility(true);
		sniperMeshComp->SetVisibility(false);
		//nowWeapon = Cast<UPrimitiveComponent>(gunMeshComp);
		break;
	case 2:
		gunMeshComp->SetVisibility(false);
		sniperMeshComp->SetVisibility(true);
		//nowWeapon = Cast<UPrimitiveComponent>(sniperMeshComp);
		break;
	case 3:
		gunMeshComp->SetVisibility(false);
		sniperMeshComp->SetVisibility(false);
		break;
	}
}
