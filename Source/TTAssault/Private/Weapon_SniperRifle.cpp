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
	Damage = 30;
	Cooltime = 5.0f;
	Ammo = 5;
	Remain = Ammo;
	reloadingTime = 5.0f;
	isCoolDown = false;
	myName = TEXT("SniperRifle");
}

void AWeapon_SniperRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeapon_SniperRifle::FireArm()
{
	//Super::FireArm();

	if (Remain <= 0 || isCoolDown)
	{
		return false;
	}
	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_SniperRifle::CoolComplete, Cooltime, false);

	//마우스 트레이스를 위한 플레이어 컨트롤러
	APlayerController* AimController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//마우스가 AimLayer에 충돌한 위치
	FHitResult result;
	AimController->GetHitResultUnderCursorForObjects(objTypes, true, result);

	//총구 Location
	FTransform temp = sniperMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//총구의 맵 평면상의 Location
	FVector temp2 = FVector(-140, temp.GetLocation().Y, temp.GetLocation().Z);
	//AimLayer = 플레이어 X 위치 반영 안돼서 -140 하드코딩, Y, Z는 마우스 방향
	FVector dir = FVector(-140, result.Location.Y, result.Location.Z);

	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(hitInfo, temp2, FVector(dir-temp2)*300000, ECollisionChannel::ECC_Visibility, params))
	{
		FTransform impactTransform(hitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, impactTransform);

		/*auto hitComp = hitInfo.GetComponent();
		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			FVector dir = (hitInfo.ImpactPoint - start).GetSafeNormal();
			FVector force = dir * hitComp->GetMass() * 500;

			hitInfo.Component->AddForceAtLocation(force, dir);
		}*/
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, this->GetActorLocation(), 1, 1, 0);

	Remain--;
	if (Remain == 0)
	{
		GetWorldTimerManager().SetTimer(autoReloadTimerHandle, this, &AWeapon_SniperRifle::MagazineReloadComplete, reloadingTime, false);
		UE_LOG(LogTemp, Warning, TEXT("Magazine"));
	}

	isCoolDown = true;
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

void AWeapon_SniperRifle::MagazineReloadComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("MagazineComplete"));
	Remain = 5;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), reloadSound, this->GetActorLocation(), 1, 1, 0);
	//GetWorldTimerManager().ClearTimer(autoReloadTimerHandle);
}

void AWeapon_SniperRifle::CoolComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("CoolDown"));
	isCoolDown = false;
	//GetWorldTimerManager().ClearTimer(autoFireTimerHandle);
}
