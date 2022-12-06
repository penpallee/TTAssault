// Fill out your copyright notice in the Description page of Project Settings.


#include "Character_Danmoozi.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include "Weapon_Pipe.h"
#include "Weapon_GrenadeLauncher.h"
#include "Weapon_SniperRifle.h"
#include <Kismet/KismetMathLibrary.h>
#include "Components/CapsuleComponent.h"

ACharacter_Danmoozi::ACharacter_Danmoozi()
{
	//Super();
	PrimaryActorTick.bCanEverTick = true;

	ACharacter_Danmoozi::HPMax = 500;
	ACharacter_Danmoozi::boosterMax = 2000;
	ACharacter_Danmoozi::speed = 900;
	selWeapon = WeaponSel::Primary;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn'"));//½ºÄÌ·¹Å» ¸Þ½Ã °¡Á®¿È
	if (tempBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempBody.Object);
		bodyMeshComp = GetMesh();
		//bodyMeshComp->SetRelativeLocation(FVector(0, 0, 0));
	}

	this->SetActorRotation(FRotator(0, 0, 0));

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	//cameraComp->SetupAttachment(springArmComp);
	//cameraComp->SetRelativeRotation(FRotator(0, -90, 0));

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacter_Danmoozi::OnCapsuleComponentBeginOverlap);
}

// Called when the game starts or when spawned
void ACharacter_Danmoozi::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;

	pipe = GetWorld()->SpawnActor<AWeapon_Pipe>(pipeFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	//pipe = CreateDefaultSubobject<AWeapon_Pipe>(TEXT("Weapon_Pipe"));
	pipe->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));//TEXT("rHand"));
	pipe->SetActorRelativeLocation(FVector(0, 0, -30));
	pipe->SetActorRelativeRotation(FRotator(30, 50, 0));

	launcher = GetWorld()->SpawnActor<AWeapon_GrenadeLauncher>(launcherFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	//launcher->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
	launcher->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));//TEXT("rHand"));
	launcher->SetActorRelativeLocation(FVector(0, 0, 0));
	launcher->SetActorRelativeRotation(FRotator(-10, 105, 0));


	rifle = GetWorld()->SpawnActor<AWeapon_SniperRifle>(rifleFactory, FTransform(GetRootComponent()->GetRelativeTransform()));
	//rifle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
	rifle->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));//TEXT("rHand"));
	rifle->SetActorRelativeLocation(FVector(-30, 0, 0));
	rifle->SetActorRelativeRotation(FRotator(-10, 105, 0));

	GetWorldTimerManager().SetTimer(autoBoosterTimerHandle, this, &ACharacter_Danmoozi::boosterCharge, 0.1f, true, 0);
}

// Called every frame
void ACharacter_Danmoozi::Tick(float DeltaTime)
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

	/*FRotator deltaRot = UKismetMathLibrary::NormalizedDeltaRotator(this->GetControlRotation(),GetActorRotation());
	FRotator makeRot = FRotator(0,mouseX,mouseY);

	FRotator interRot = UKismetMathLibrary::RInterpTo(makeRot, deltaRot,GetWorld()->GetDeltaSeconds(),15);
	this->SetActorRotation(interRot);*/

	//FHitResult result;
	//GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1,false,result );
	//this->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(result.Location,GetActorLocation()));
}

// Called to bind functionality to input
void ACharacter_Danmoozi::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AAssaultCharacter::onActionBoost);
	InputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AAssaultCharacter::OnAxisHorizontal);
	InputComponent->BindAxis(TEXT("Move Right / Left"), this, &AAssaultCharacter::OnAxisVertical);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &AAssaultCharacter::onAxisMouseY);
	InputComponent->BindAxis("Turn Right / Left", this, &AAssaultCharacter::onAxisMouseX);
	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AAssaultCharacter::OnActionFire);
	InputComponent->BindAction(TEXT("WeaponPrimary"), IE_Pressed, this, &AAssaultCharacter::onSelPrimary);
	InputComponent->BindAction(TEXT("WeaponSecondary"), IE_Pressed, this, &AAssaultCharacter::onSelSecondary);
	InputComponent->BindAction(TEXT("WeaponTertiary"), IE_Pressed, this, &AAssaultCharacter::onSelTetertiary);
}

void ACharacter_Danmoozi::OnActionFire()
{
	switch (selWeapon)
	{
	case WeaponSel::Primary:
		if (pipe->FireArm())
		{
			FString sectionName = FString::Printf(TEXT("Attack_Melee%d"), pipe->GetCombo());
			PlayAnimMontage(attackAnimMontage, 2, FName(*sectionName));
			pipe->SetCombo();
			UE_LOG(LogTemp, Warning, TEXT("%d"), pipe->GetCombo());
		}
		break;
	case WeaponSel::Secondary:
		if (launcher->FireArm())
			PlayAnimMontage(attackAnimMontage, 1, TEXT("Fire"));
		break;
	case WeaponSel::Tertiary:
		if (rifle->FireArm())
			PlayAnimMontage(attackAnimMontage, 1, TEXT("Fire_Rifle"));
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
	//AddControllerYawInput(-value);
	//mouseX -= value;
}

void ACharacter_Danmoozi::onAxisMouseY(float value)
{
	Super::onAxisMouseY(value);
	//AddControllerRollInput(value);
	//mouseY += value;
	//AddControllerYawInput(value);
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

FPlayerStatus ACharacter_Danmoozi::returnStatus()
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
		nowStat.ammo = launcher->returnAmmo();
		nowStat.WeaponName = launcher->returnName();
		nowStat.magazine = launcher->returnMagazine();
		nowStat.coolTime = launcher->returnCoolTime();
		nowStat.coolTimeProgress = launcher->returnCoolProgress();
		nowStat.reloadTime = launcher->returnReloadTime();
		nowStat.reloadProgress = launcher->returnReloadProgress();
		break;
	case WeaponSel::Tertiary:
		nowStat.ammo = rifle->returnAmmo();
		nowStat.WeaponName = rifle->returnName();
		nowStat.magazine = rifle->returnMagazine();
		nowStat.coolTime = rifle->returnCoolTime();
		nowStat.coolTimeProgress = rifle->returnCoolProgress();
		nowStat.reloadTime = rifle->returnReloadTime();
		nowStat.reloadProgress = rifle->returnReloadProgress();
		break;
	}
	nowStat.HPMax = this->HPMax;
	nowStat.HP = this->HP;
	nowStat.boostMax = this->boosterMax;
	nowStat.boost = this->booster;

	return nowStat;
}

void ACharacter_Danmoozi::boosterCharge()
{
	if (booster < boosterMax)
		booster += 10;
	else
		booster = boosterMax;
}