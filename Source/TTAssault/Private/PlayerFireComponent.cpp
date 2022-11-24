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
	//FirePosition->SetRelativeRotation(FRotator(90, 0, 0));
	FirePosition->SetupAttachment(this);

	this->SetRelativeLocation(FVector(14, 25, 90));
	//this->SetRelativeRotation(FRotator(0,-90,0));

	meleeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meleeMeshComp"));
	//meleeMeshComp->AttachToComponent(GetOwner()->GetRootComponent(),FAttachmentTransformRules::KeepWorldTransform,("rHand"));
	//meleeMeshComp->SetupAttachment(GetOwner()->GetRootComponent());
	meleeMeshComp->SetupAttachment(this);

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	gunMeshComp->SetupAttachment(this);

	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperMeshComp"));
	sniperMeshComp->SetupAttachment(this);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("SkeletalMesh'/Game/TH/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/TH/Resources/SniperGun/Sniper.Sniper'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMelee(TEXT("StaticMesh'/Game/StarterContent/Props/SM_PillarFrame.SM_PillarFrame'"));



	if (tempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGun.Object);
		gunMeshComp->SetVisibility(false);
	}

	if (tempSniper.Succeeded())
	{
		sniperMeshComp->SetStaticMesh(tempSniper.Object);
		sniperMeshComp->SetRelativeScale3D(FVector(.15f));
		sniperMeshComp->SetVisibility(false);
	}

	if (tempMelee.Succeeded())
	{
		meleeMeshComp->SetStaticMesh(tempMelee.Object);
		meleeMeshComp->SetRelativeRotation(FRotator(90, 0, 0));
		meleeMeshComp->SetRelativeScale3D(FVector(.15f));

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
	FTransform	t;
	switch (selWeapon)
	{
	case WeponSel::PRIMARY:
		break;
	case WeponSel::SECONDARY:
		t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<AMyGrenade>(bulletFactory, t);

		UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
		break;
	case WeponSel::TERTIARY:
		Sniping();
		break;
	}
}

void UPlayerFireComponent::onSelPrimary()
{
	SetGun(WeponSel::PRIMARY);
	selWeapon = WeponSel::PRIMARY;
}

void UPlayerFireComponent::onSelSecondary()
{
	SetGun(WeponSel::SECONDARY);
	selWeapon = WeponSel::SECONDARY;
}

void UPlayerFireComponent::onSelTetertiary()
{
	SetGun(WeponSel::TERTIARY);
	selWeapon = WeponSel::TERTIARY;
}


void UPlayerFireComponent::SetGun(WeponSel num)
{
	switch (num)
	{
	case WeponSel::PRIMARY:
		meleeMeshComp->SetVisibility(true);
		meleeMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		gunMeshComp->SetVisibility(false);
		gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		sniperMeshComp->SetVisibility(false);
		sniperMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case WeponSel::SECONDARY:
		meleeMeshComp->SetVisibility(false);
		meleeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		gunMeshComp->SetVisibility(true);
		gunMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		sniperMeshComp->SetVisibility(false);
		sniperMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;

	case WeponSel::TERTIARY:
		meleeMeshComp->SetVisibility(false);
		meleeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		gunMeshComp->SetVisibility(false);
		gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		sniperMeshComp->SetVisibility(true);
		sniperMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		//nowWeapon = Cast<UPrimitiveComponent>(sniperMeshComp);
		break;
	}
}

void UPlayerFireComponent::Sniping()
{
	FVector start = sniperMeshComp->GetSocketLocation(TEXT("FirePosition"));
	FVector end = start + sniperMeshComp->GetSocketLocation(TEXT("FirePosition")) * 300000.0f;
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
