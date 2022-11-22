// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"
#include "AssaultCharacter.h"
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Actor.h>

// Sets default values for this component's properties
UPlayerFireComponent::UPlayerFireComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FirePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition"));
	FirePosition->SetRelativeLocation(FVector(0, 0, 80), false, false, ETeleportType::None);
	FirePosition->SetRelativeRotation(FRotator(90, 0, 0));

	meleeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meleeMeshComp"));
	//meleeMeshComp->SetupAttachment(GetOwner()->scene);

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	//gunMeshComp->SetupAttachment(GetOwner()->GetRootComponent());

	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperMeshComp"));
	//sniperMeshComp->SetupAttachment(GetOwner()->GetRootComponent());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("SkeletalMesh'/Game/TH/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/TH/Resources/SniperGun/Sniper.Sniper'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMelee(TEXT("StaticMesh'/Game/StarterContent/Props/SM_PillarFrame.SM_PillarFrame'"));


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

	if (tempMelee.Succeeded())
	{
		meleeMeshComp->SetStaticMesh(tempMelee.Object);
		meleeMeshComp->SetRelativeScale3D(FVector(.15f));
		meleeMeshComp->SetRelativeLocation(FVector(-170, 10, 100));
	}
}


// Called when the game starts
void UPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();

	myOwner = Cast<AAssaultCharacter>(GetOwner());
}


// Called every frame
void UPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerFireComponent::SetupFire(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFireComponent::OnActionFire);
	PlayerInputComponent->BindAction(TEXT("WeaponPrimary"), IE_Pressed, this, &UPlayerFireComponent::onSelPrimary);
	PlayerInputComponent->BindAction(TEXT("WeaponSecondary"), IE_Pressed, this, &UPlayerFireComponent::onSelSecondary);
	PlayerInputComponent->BindAction(TEXT("WeaponTertiary"), IE_Pressed, this, &UPlayerFireComponent::onSelTetertiary);
}

void UPlayerFireComponent::OnActionFire()
{
	GetWorld()->SpawnActor<AGrenade>(bulletFactory, FirePosition->GetComponentTransform());

	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}

void UPlayerFireComponent::onSelPrimary()
{
	SetGun(PRIMARY);
	selWeapon = PRIMARY;
}

void UPlayerFireComponent::onSelSecondary()
{
	SetGun(SECONDARY);
	selWeapon = SECONDARY;
}

void UPlayerFireComponent::onSelTetertiary()
{
	SetGun(TERTIARY);
	selWeapon = TERTIARY;
}


void UPlayerFireComponent::SetGun(int num)
{
	switch (num)
	{
	case PRIMARY:
		meleeMeshComp->SetVisibility(true);
		gunMeshComp->SetVisibility(false);
		sniperMeshComp->SetVisibility(false);
		break;
	case SECONDARY:
		meleeMeshComp->SetVisibility(false);
		gunMeshComp->SetVisibility(true);
		sniperMeshComp->SetVisibility(false);
		//nowWeapon = Cast<UPrimitiveComponent>(gunMeshComp);
		break;
	case TERTIARY:
		meleeMeshComp->SetVisibility(false);
		gunMeshComp->SetVisibility(false);
		sniperMeshComp->SetVisibility(true);
		//nowWeapon = Cast<UPrimitiveComponent>(sniperMeshComp);
		break;
	}
}

void UPlayerFireComponent::Sniping()
{
	FVector start = sniperMeshComp->GetComponentLocation();
	FVector end = start + sniperMeshComp->GetForwardVector() * 300000.0f;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params))
	{
		FTransform impactTransform(hitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, impactTransform);

		auto hitComp = hitInfo.GetComponent();
		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			FVector dir = (hitInfo.ImpactPoint - start).GetSafeNormal();
			FVector force = dir * hitComp->GetMass() * 500000;

			hitInfo.Component->AddForceAtLocation(force, dir);
		}
	}
}
