// Fill out your copyright notice in the Description page of Project Settings.


#include "ChildAssaultCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

AChildAssaultCharacter::AChildAssaultCharacter()
{
	//Super();
}

// Called when the game starts or when spawned
void AChildAssaultCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AChildAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AChildAssaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AChildAssaultCharacter::OnAxisHorizontal(float value)
{
	Super::OnAxisHorizontal(value);
}

void AChildAssaultCharacter::OnAxisVertical(float value)
{
	Super::OnAxisVertical(value);
}

void AChildAssaultCharacter::onActionBoost()
{
	Super::onActionBoost();
}

void AChildAssaultCharacter::onActionFire()
{
	Super::onActionFire();
	switch (selWeapon)
	{
		case PRIMARY:
		break;
		case SECONDARY:
		Sniping();
		break;
		case TERTIARY:
		break;
	}
}

void AChildAssaultCharacter::OnPlayerHit(int damage)
{
	Super::OnPlayerHit(damage);
}

void AChildAssaultCharacter::Stop()
{
	Super::Stop();
}

void AChildAssaultCharacter::onSelPrimary()
{
	Super::onSelPrimary();
	selWeapon=PRIMARY;
}

void AChildAssaultCharacter::onSelSecondary()
{
	Super::onSelSecondary();
	selWeapon = SECONDARY;
}

void AChildAssaultCharacter::onSelTeTertiary()
{
	Super::onSelTeTertiary();
	selWeapon = TERTIARY;
}

void AChildAssaultCharacter::SetGun(int num)
{
	Super::SetGun(num);
}

void AChildAssaultCharacter::onAxisMouseX(float value)
{
	Super::onAxisMouseX(value);
}

void AChildAssaultCharacter::onAxisMouseY(float value)
{
	Super::onAxisMouseY(value);
}

void AChildAssaultCharacter::Sniping()
{
	FVector start = sniperMeshComp->GetComponentLocation();
	FVector end = start + sniperMeshComp->GetForwardVector() * 300000.0f;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
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
