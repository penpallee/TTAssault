// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Soondae.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Weapon_Pipe.h"
#include "Weapon_MachineGun.h"
#include "Weapon_BeamRifle.h"
#include <Kismet/KismetMathLibrary.h>
#include "Components/CapsuleComponent.h"

ACharacter_Soondae::ACharacter_Soondae()
{
	//Super();
	PrimaryActorTick.bCanEverTick = true;

	ACharacter_Soondae::HPMax = 100;
	ACharacter_Soondae::boosterMax = 2000;
	ACharacter_Soondae::speed = 1000;
	selWeapon = WeaponSel::Primary;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn'"));//½ºÄÌ·¹Å» ¸Þ½Ã °¡Á®¿È
	if (tempBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempBody.Object);
		bodyMeshComp = GetMesh();
	}

	this->SetActorRotation(FRotator(0, 0, 0));

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Soondae::OnCapsuleComponentBeginOverlap);
}

// Called when the game starts or when spawned
void ACharacter_Soondae::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;

	pipe = GetWorld()->SpawnActor<AWeapon_Pipe>(pipeFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	pipe->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));//TEXT("rHand"));
	pipe->SetActorRelativeLocation(FVector(0, 0, -30));
	pipe->SetActorRelativeRotation(FRotator(30, 50, 0));

	machineGun = GetWorld()->SpawnActor<AWeapon_MachineGun>(machineGunFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	machineGun->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
	machineGun->SetActorRelativeLocation(FVector(0, 0, 0));
	machineGun->SetActorRelativeRotation(FRotator(-10, 105, 0));

	beamRifle = GetWorld()->SpawnActor<AWeapon_BeamRifle>(beamRifleFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	beamRifle->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
	beamRifle->SetActorRelativeLocation(FVector(-30, 0, 0));
	beamRifle->SetActorRelativeRotation(FRotator(-10, 105, 0));

	GetWorldTimerManager().SetTimer(autoBoosterTimerHandle, this, &ACharacter_Soondae::boosterCharge, 0.1f, true, 0);
}

// Called every frame
void ACharacter_Soondae::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector t = this->GetActorLocation();


	if (bTing && tingCurrentTime > 0)
	{
		tingCurrentTime -= DeltaTime;
		SetActorLocation(GetActorLocation() + tingDirection * tingSpeed * DeltaTime);
		if (tingCurrentTime <= 0)
		{
			bTing = false;
			tingCurrentTime = 0;
		}
	}
	else
	{
		FHitResult hitInfo;

		this->SetActorLocation(GetActorLocation() + direction * speed * DeltaTime, true, &hitInfo);

		auto hitActor = hitInfo.GetActor();
		if (hitActor)
		{
			tingDirection = hitInfo.ImpactNormal;
			tingDirection.X = 0;
			tingDirection.Normalize();

			tingCurrentTime = tingTime;
			bTing = true;

			if (hitActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *hitActor->GetName());
			}
		}
	}
	cameraComp->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(cameraComp->GetRelativeLocation(), this->GetActorLocation()));
	cameraComp->SetRelativeLocation(FVector(-1000, t.Y * 0.95f, t.Z * 0.95f));
}

// Called to bind functionality to input
void ACharacter_Soondae::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AAssaultCharacter::onActionBoost);
	InputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AAssaultCharacter::OnAxisHorizontal);
	InputComponent->BindAxis(TEXT("Move Right / Left"), this, &AAssaultCharacter::OnAxisVertical);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &AAssaultCharacter::onAxisMouseY);
	InputComponent->BindAxis("Turn Right / Left", this, &AAssaultCharacter::onAxisMouseX);
	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AAssaultCharacter::OnActionFire);
	InputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AAssaultCharacter::OnActionStop);
	InputComponent->BindAction(TEXT("WeaponPrimary"), IE_Pressed, this, &AAssaultCharacter::onSelPrimary);
	InputComponent->BindAction(TEXT("WeaponSecondary"), IE_Pressed, this, &AAssaultCharacter::onSelSecondary);
	InputComponent->BindAction(TEXT("WeaponTertiary"), IE_Pressed, this, &AAssaultCharacter::onSelTetertiary);
}

void ACharacter_Soondae::OnActionFire()
{
	switch (selWeapon)
	{
	case WeaponSel::Primary:
		if (pipe->FireArm())
		{
			FString sectionName = FString::Printf(TEXT("Attack_Melee%d"), pipe->GetCombo());
			PlayAnimMontage(attackAnimMontage, 2, FName(*sectionName));
			pipe->SetCombo();
		}
		break;
	case WeaponSel::Secondary:
		if (machineGun->FireArm())
			PlayAnimMontage(attackAnimMontage, 1, TEXT("Fire"));
		break;
	case WeaponSel::Tertiary:
		if (beamRifle->FireArm())
			PlayAnimMontage(attackAnimMontage, 1, TEXT("Fire_Rifle"));
		break;
	}
}

void ACharacter_Soondae::OnActionStop()
{
	machineGun->FireStop();
}

void ACharacter_Soondae::OnAxisHorizontal(float value)
{
	Super::OnAxisHorizontal(value);
	direction.Z = value;
}

void ACharacter_Soondae::OnAxisVertical(float value)
{
	Super::OnAxisVertical(value);
	direction.Y = value;
}

void ACharacter_Soondae::onActionBoost()
{
	Super::onActionBoost();
}

void ACharacter_Soondae::OnPlayerHit(int damage)
{
	Super::OnPlayerHit(damage);
}

void ACharacter_Soondae::Stop()
{
	Super::Stop();
}

void ACharacter_Soondae::onAxisMouseX(float value)
{
	Super::onAxisMouseX(value);
	//AddControllerYawInput(-value);
	//mouseX -= value;
}

void ACharacter_Soondae::onAxisMouseY(float value)
{
	Super::onAxisMouseY(value);
	//AddControllerRollInput(value);
	//mouseY += value;
	//AddControllerYawInput(value);
}

void ACharacter_Soondae::onSelPrimary()
{
	selWeapon = WeaponSel::Primary;

	pipe->OnAwake();
	machineGun->OnSleep();
	beamRifle->OnSleep();
}

void ACharacter_Soondae::onSelSecondary()
{
	selWeapon = WeaponSel::Secondary;

	pipe->OnSleep();
	machineGun->OnAwake();
	beamRifle->OnSleep();
}

void ACharacter_Soondae::onSelTetertiary()
{
	selWeapon = WeaponSel::Tertiary;

	pipe->OnSleep();
	machineGun->OnSleep();
	beamRifle->OnAwake();
}

FPlayerStatus ACharacter_Soondae::returnStatus()
{
	FPlayerStatus nowStat;

	switch (selWeapon)
	{
	case WeaponSel::Primary:
		nowStat.ammo = pipe->returnAmmo();
		nowStat.WeaponName = pipe->returnName();
		nowStat.magazine = pipe->returnMagazine();
		nowStat.coolTime = pipe->returnCoolTime();
		nowStat.coolTimeProgress = pipe->returnCoolProgress();
		nowStat.reloadTime = pipe->returnReloadTime();
		nowStat.reloadProgress = pipe->returnReloadProgress();
		break;
	case WeaponSel::Secondary:
		nowStat.ammo = machineGun->returnAmmo();
		nowStat.WeaponName = machineGun->returnName();
		nowStat.magazine = machineGun->returnMagazine();
		nowStat.coolTime = machineGun->returnCoolTime();
		nowStat.coolTimeProgress = machineGun->returnCoolProgress();
		nowStat.reloadTime = machineGun->returnReloadTime();
		nowStat.reloadProgress = machineGun->returnReloadProgress();
		break;
	case WeaponSel::Tertiary:
		nowStat.ammo = beamRifle->returnAmmo();
		nowStat.WeaponName = beamRifle->returnName();
		nowStat.magazine = beamRifle->returnMagazine();
		nowStat.coolTime = beamRifle->returnCoolTime();
		nowStat.coolTimeProgress = beamRifle->returnCoolProgress();
		nowStat.reloadTime = beamRifle->returnReloadTime();
		nowStat.reloadProgress = beamRifle->returnReloadProgress();
		break;
	}
	nowStat.HPMax = this->HPMax;
	nowStat.HP = this->HP;
	nowStat.boostMax = this->boosterMax;
	nowStat.boost = this->booster;

	return nowStat;
}

void ACharacter_Soondae::boosterCharge()
{
	if (booster < boosterMax)
		booster += 10;
	else
		booster = boosterMax;
}