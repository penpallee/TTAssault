// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_SniperRifle.h"
#include <Kismet/GameplayStatics.h>

AWeapon_SniperRifle::AWeapon_SniperRifle()
{
	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperMeshComp"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/TH/Resources/SniperGun/Sniper.Sniper'"));

	if (tempSniper.Succeeded())
	{
		sniperMeshComp->SetStaticMesh(tempSniper.Object);
		sniperMeshComp->SetRelativeScale3D(FVector(.15f));
		sniperMeshComp->SetVisibility(false);
	}

	//this->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
}

void AWeapon_SniperRifle::BeginPlay()
{
	Super::BeginPlay();
	Damage = 20;
	Cooltime = 20.0f;
	Ammo = 20;
	Remain = Ammo;
	myName = TEXT("SniperRifle");
}

void AWeapon_SniperRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeapon_SniperRifle::FireArm()
{
	//Super::FireArm();
	if (Remain <= 0)
		return false;

	FVector start = sniperMeshComp->GetSocketLocation(TEXT("FirePosition"));
	FVector end = start + sniperMeshComp->GetRightVector() * 300000.0f;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params))
	{
		FTransform impactTransform(hitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, impactTransform);

		/*auto hitComp = hitInfo.GetComponent();
		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			FVector dir = (hitInfo.ImpactPoint - start).GetSafeNormal();
			FVector force = dir * hitComp->GetMass() * 500000;

			hitInfo.Component->AddForceAtLocation(force, dir);
		}*/
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),fireSound,this->GetActorLocation(),1,1,0);
	Remain--;
	return true;
}

void AWeapon_SniperRifle::OnSleep()
{
	sniperMeshComp->SetVisibility(false);
	sniperMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
void AWeapon_SniperRifle::OnAwake()
{
	sniperMeshComp->SetVisibility(true);
	sniperMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

int AWeapon_SniperRifle::returnAmmo()
{
	return Remain;
}

FString AWeapon_SniperRifle::returnName()
{
	return myName;
}

void AWeapon_SniperRifle::SetTimerMagazineReload()
{

}

void AWeapon_SniperRifle::MagazineReloadComplete()
{

}

void AWeapon_SniperRifle::SetTimerCoolDown()
{

}
