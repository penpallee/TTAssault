// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Danmoozi.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "PlayerFireComponent.h"
#include "Weapon_Pipe.h"
#include "Weapon_GrenadeLauncher.h"
#include "Weapon_SniperRifle.h"

ACharacter_Danmoozi::ACharacter_Danmoozi()
{
	//Super();
	PrimaryActorTick.bCanEverTick = true;
	selWeapon= WeaponSel::Primary;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn'"));//���̷�Ż �޽� ������
	if (tempBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempBody.Object);
		bodyMeshComp = GetMesh();
		//bodyMeshComp->SetRelativeLocation(FVector(0, 0, 0));
	}

	this->SetActorRotation(FRotator(0, 0, 0));

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	springArmComp->SetupAttachment((RootComponent));
	springArmComp->SetRelativeLocation(FVector(0, 0, 0));
	springArmComp->TargetArmLength = 1000;

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	cameraComp->SetupAttachment(springArmComp);
	//cameraComp->SetRelativeRotation(FRotator(0, -90, 0));

	//pipe = CreateDefaultSubobject<AWeapon_Pipe>(TEXT("Weapon_Pipe"));
	//pipe = GetWorld()->SpawnActor<AWeapon_Pipe>(pipeFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	//pipe->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform,NAME_None);//TEXT("rHand"));

	//launcher = CreateDefaultSubobject<AWeapon_GrenadeLauncher>(TEXT("Weapon_GL"));
	//launcher->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);

	//rifle = CreateDefaultSubobject<AWeapon_SniperRifle>(TEXT("Weapon_SR"));
	//rifle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
}

// Called when the game starts or when spawned
void ACharacter_Danmoozi::BeginPlay()
{
	Super::BeginPlay();
	InputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AAssaultCharacter::onActionBoost);
	InputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AAssaultCharacter::OnAxisHorizontal);
	InputComponent->BindAxis(TEXT("Move Right / Left"), this, &AAssaultCharacter::OnAxisVertical);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &AAssaultCharacter::onAxisMouseY);
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &AAssaultCharacter::onAxisMouseX);
	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AAssaultCharacter::OnActionFire);
	InputComponent->BindAction(TEXT("WeaponPrimary"), IE_Pressed, this, &AAssaultCharacter::onSelPrimary);
	InputComponent->BindAction(TEXT("WeaponSecondary"), IE_Pressed, this, &AAssaultCharacter::onSelSecondary);
	InputComponent->BindAction(TEXT("WeaponTertiary"), IE_Pressed, this, &AAssaultCharacter::onSelTetertiary);
	speed = 500;
	booster = 1000;
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;

	pipe = GetWorld()->SpawnActor<AWeapon_Pipe>(pipeFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	pipe->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform, NAME_None);//TEXT("rHand"));

	launcher = GetWorld()->SpawnActor<AWeapon_GrenadeLauncher>(launcherFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	launcher->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);

	rifle = GetWorld()->SpawnActor<AWeapon_SniperRifle>(rifleFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	rifle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, NAME_None);
}

// Called every frame
void ACharacter_Danmoozi::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	this->SetActorLocation(GetActorLocation() + direction * speed * DeltaTime);
}

// Called to bind functionality to input
void ACharacter_Danmoozi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACharacter_Danmoozi::OnActionFire()
{
	switch (selWeapon)
	{
	case WeaponSel::Primary:
		pipe->FireArm();
		break;
	case WeaponSel::Secondary:
		launcher->FireArm();
		break;
	case WeaponSel::Tertiary:
		rifle->FireArm();
		break;
	}
}

void ACharacter_Danmoozi::OnAxisHorizontal(float value)
{
	Super::OnAxisHorizontal(value);
	direction.Z = value;
}

void ACharacter_Danmoozi::OnAxisVertical(float value)
{
	Super::OnAxisVertical(value);
	direction.Y = value;
}

void ACharacter_Danmoozi::onActionBoost()
{
	Super::onActionBoost();
}

void ACharacter_Danmoozi::OnPlayerHit(int damage)
{
	Super::OnPlayerHit(damage);
}

void ACharacter_Danmoozi::Stop()
{
	Super::Stop();
}

void ACharacter_Danmoozi::onAxisMouseX(float value)
{
	Super::onAxisMouseX(value);
}

void ACharacter_Danmoozi::onAxisMouseY(float value)
{
	Super::onAxisMouseY(value);
}

void ACharacter_Danmoozi::onSelPrimary()
{
	selWeapon = WeaponSel::Primary;

	pipe->OnAwake();
	launcher->OnSleep();
	rifle->OnSleep();
}

void ACharacter_Danmoozi::onSelSecondary()
{
	selWeapon = WeaponSel::Secondary;

	pipe->OnSleep();
	launcher->OnAwake();
	rifle->OnSleep();
}

void ACharacter_Danmoozi::onSelTetertiary()
{
	selWeapon = WeaponSel::Tertiary;

	pipe->OnSleep();
	launcher->OnSleep();
	rifle->OnAwake();
}