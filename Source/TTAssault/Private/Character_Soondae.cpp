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

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn'"));//???̷?Ż ?޽? ??????
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

	bReplicates = true;
}

void ACharacter_Soondae::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacter_Soondae, soondaeOwner);
	DOREPLIFETIME(ACharacter_Soondae, HP);
	DOREPLIFETIME(ACharacter_Soondae, bodyMeshComp);
}

// Called when the game starts or when spawned
void ACharacter_Soondae::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;

	pipe = GetWorld()->SpawnActor<AWeapon_Pipe>(pipeFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	pipe->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));//TEXT("rHand"));
	//pipe->SetActorRelativeLocation(FVector(-22, -10, 7));
	//pipe->SetActorRelativeRotation(FRotator(70, 20, -160));
	pipe->SetActorRelativeLocation(FVector(-815, -2350, -404));
	pipe->SetActorRelativeRotation(FRotator(60, 170, -82));

	machineGun = GetWorld()->SpawnActor<AWeapon_MachineGun>(machineGunFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	machineGun->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
	machineGun->SetActorRelativeLocation(FVector(-534, -22, -734));
	machineGun->SetActorRelativeRotation(FRotator(-44, 20, 21));

	beamRifle = GetWorld()->SpawnActor<AWeapon_BeamRifle>(beamRifleFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	beamRifle->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
	beamRifle->SetActorRelativeLocation(FVector(-3210, 6400, -1550));
	beamRifle->SetActorRelativeRotation(FRotator(-83, -100, -230));

	GetWorldTimerManager().SetTimer(autoBoosterTimerHandle, this, &ACharacter_Soondae::boosterCharge, 0.1f, true, 0);
}

// Called every frame
void ACharacter_Soondae::Tick/*_Implementation*/(float DeltaTime)
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
		}
	}
	cameraComp->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(cameraComp->GetRelativeLocation(), this->GetActorLocation()));
	cameraComp->SetRelativeLocation(FVector(-850, t.Y * 0.95f, t.Z * 0.95f));
}

// Called to bind functionality to input
void ACharacter_Soondae::SetupPlayerInputComponent/*_Implementation*/(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &ACharacter_Soondae::onActionBoostMulticast);
	InputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ACharacter_Soondae::OnAxisHorizontalMulticast);
	InputComponent->BindAxis(TEXT("Move Right / Left"), this, &ACharacter_Soondae::OnAxisVerticalMulticast);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &ACharacter_Soondae::onAxisMouseYMulticast);
	InputComponent->BindAxis("Turn Right / Left", this, &ACharacter_Soondae::onAxisMouseXMulticast);
	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ACharacter_Soondae::OnActionFireMultiCast);
	InputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ACharacter_Soondae::OnActionStopMulticast);
	InputComponent->BindAction(TEXT("WeaponPrimary"), IE_Pressed, this, &ACharacter_Soondae::onSelPrimaryMulticast);
	InputComponent->BindAction(TEXT("WeaponSecondary"), IE_Pressed, this, &ACharacter_Soondae::onSelSecondaryMulticast);
	InputComponent->BindAction(TEXT("WeaponTertiary"), IE_Pressed, this, &ACharacter_Soondae::onSelTetertiaryMulticast);
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::OnActionFire_Implementation()
{
	switch (selWeapon)
	{
	case WeaponSel::Primary:
		if (pipe->FireArm())
		{
			FString sectionName = FString::Printf(TEXT("Attack_Melee%d"), pipe->GetCombo());
			PlayAnimMontage(attackAnimMontage, 1.7f, FName(*sectionName));
			pipe->SetCombo();
		}
		break;
	case WeaponSel::Secondary:
		if (machineGun->FireArm())
			//PlayAnimMontage(attackAnimMontage, 1, TEXT("Fire_Gun"));
			PlayAnimMontage(attackAnimMontage, 1, TEXT("Fire"));
		break;
	case WeaponSel::Tertiary:
		if (beamRifle->FireArm())
			PlayAnimMontage(attackAnimMontage, 1, TEXT("Fire_Beam"));
		break;
	}
}

void ACharacter_Soondae::OnActionFireMultiCast_Implementation()
{
	OnActionFire();
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::OnActionStop_Implementation()
{
	machineGun->FireStop();
	if(selWeapon == WeaponSel::Secondary)
		StopAnimMontage(attackAnimMontage);
}	

void ACharacter_Soondae::OnActionStopMulticast_Implementation()
{
	OnActionStop();
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::OnAxisHorizontal_Implementation(float value)
{
	Super::OnAxisHorizontal(value);
	direction.Z = value;
}

void ACharacter_Soondae::OnAxisHorizontalMulticast_Implementation(float value)
{
	OnAxisHorizontal(value);
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::OnAxisVertical_Implementation(float value)
{
	Super::OnAxisVertical(value);
	direction.Y = value;
}

void ACharacter_Soondae::OnAxisVerticalMulticast_Implementation(float value)
{
	OnAxisVertical(value);
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::onActionBoost_Implementation()
{
	Super::onActionBoost();
}

void ACharacter_Soondae::onActionBoostMulticast_Implementation()
{
	onActionBoost();
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::OnPlayerHit_Implementation(int damage)
{
	Super::OnPlayerHit(damage);
}

void ACharacter_Soondae::OnPlayerHitMulticast_Implementation(int damage)
{
	OnPlayerHit(damage);
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::Stop/*_Implementation*/()
{
	Super::Stop();
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::onAxisMouseX_Implementation(float value)
{
	Super::onAxisMouseX(value);
	//AddControllerYawInput(-value);
	//mouseX -= value;
}

void ACharacter_Soondae::onAxisMouseXMulticast_Implementation(float value)
{
	onAxisMouseX(value);
}

bool ACharacter_Soondae::onAxisMouseX_Validate(float value)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::onAxisMouseY_Implementation(float value)
{
	Super::onAxisMouseY(value);
	//AddControllerRollInput(value);
	//mouseY += value;
	//AddControllerYawInput(value);
}

void ACharacter_Soondae::onAxisMouseYMulticast_Implementation(float value)
{
	onAxisMouseY(value);
}

bool ACharacter_Soondae::onAxisMouseY_Validate(float value)
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::onSelPrimary_Implementation()
{
	selWeapon = WeaponSel::Primary;

	pipe->OnAwake();
	machineGun->OnSleep();
	beamRifle->OnSleep();
}

void ACharacter_Soondae::onSelPrimaryMulticast_Implementation()
{
	onSelPrimary();
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::onSelSecondary_Implementation()
{
	selWeapon = WeaponSel::Secondary;

	pipe->OnSleep();
	machineGun->OnAwake();
	beamRifle->OnSleep();
}

void ACharacter_Soondae::onSelSecondaryMulticast_Implementation()
{
	onSelSecondary();
}

////////////////////////////////////////////////////////////////////////////////////

void ACharacter_Soondae::onSelTetertiary_Implementation()
{
	selWeapon = WeaponSel::Tertiary;

	pipe->OnSleep();
	machineGun->OnSleep();
	beamRifle->OnAwake();
}

void ACharacter_Soondae::onSelTetertiaryMulticast_Implementation()
{
	onSelTetertiary();
}

////////////////////////////////////////////////////////////////////////////////////

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