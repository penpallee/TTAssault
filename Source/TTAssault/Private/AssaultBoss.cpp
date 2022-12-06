// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultBoss.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Engine/SkeletalMesh.h>
#include <Kismet/KismetMathLibrary.h>
#include "AIController.h"
#include "Character_Danmoozi.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AAssaultBoss::AAssaultBoss()
{
	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmpclass(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));

	if (tmpclass.Succeeded())
	{
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, 90, 0));
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->GravityScale = 0;
		GetCharacterMovement()->DefaultLandMovementMode = EMovementMode::MOVE_Flying;
		GetCharacterMovement()->bUseSeparateBrakingFriction = true;
		GetCharacterMovement()->BrakingFriction = 1.0f;

		springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
		cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
		
		springArmComp->SetupAttachment(RootComponent);
		springArmComp->TargetArmLength = 1000;
		springArmComp->bInheritPitch = false;
		springArmComp->bInheritRoll = false;
		springArmComp->bInheritYaw = false;

		cameraComp->SetupAttachment(springArmComp);

	

		particleComp1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LeftFootBoostFire"));
		particleComp2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RightFootBoostFire"));

		particleComp1->SetupAttachment(GetMesh());
		particleComp2->SetupAttachment(GetMesh());

		particleComp1->SetRelativeLocationAndRotation(FVector(50, 30, -5), FRotator(90, 90, 0));
		particleComp2->SetRelativeLocationAndRotation(FVector(-50, -30, 5), FRotator(90, 90, 0));

		BossMoveComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BossMoveComp"));

		BossMoveComp->bRotationFollowsVelocity = true;
		BossMoveComp->ProjectileGravityScale = 0;
		BossMoveComp->bInitialVelocityInLocalSpace = true;
		BossMoveComp->bShouldBounce = true;
		BossMoveComp->Bounciness = 0.7;
		BossMoveComp->Friction = 0.2;
		BossMoveComp->bForceSubStepping = true;
		BossMoveComp->bIsHomingProjectile = true;
		BossMoveComp->HomingAccelerationMagnitude = 1000;
		BossMoveComp->bInterpMovement = true;
		BossMoveComp->bInterpRotation = true;
		BossMoveComp->SetPlaneConstraintNormal(FVector(1, 0, 0));
		BossMoveComp->bConstrainToPlane = true;
		BossMoveComp->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);

		GetCapsuleComponent()->SetGenerateOverlapEvents(true);
		GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetCapsuleComponent()->SetCollisionProfileName(TEXT("Boss"));
		GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AAssaultBoss::OnCapsuleComponentBeginOverlap);
	}

	Defense = 0;

}

// Called when the game starts or when spawned
void AAssaultBoss::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAssaultBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAssaultBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AAssaultBoss::OnAxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &AAssaultBoss::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Turn R/L"), this, &AAssaultBoss::OnAxisHorizontalView);
}

void AAssaultBoss::OnBossHit(int damage)
{
	HP -= (damage - Defense);
	if (HP <= 0)
	{
		Destroy();
	}
}

void AAssaultBoss::OnAxisVertical(float value)
{
	AddMovementInput(UKismetMathLibrary::GetUpVector(FRotator(0, 0, 0)), value);
}

void AAssaultBoss::OnAxisHorizontal(float value)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, 0, 0)), value);
}

void AAssaultBoss::OnAxisHorizontalView(float value)
{
	SetActorRotation(FRotator(0, (-90) * value, 0));
}

void AAssaultBoss::OnCapsuleComponentBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacter_Danmoozi::StaticClass()))
	{
		UKismetSystemLibrary::PrintString(GetWorld(), TEXT("Boss Damage to Player By Physical 5"));
		Cast<ACharacter_Danmoozi>(OtherActor)->OnPlayerHit(5);
	}
}
