// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_SniperRifle.h"
#include <Kismet/GameplayStatics.h>

#include "AssaultBoss.h"

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
	damage = 30;
	coolTime = 5.0f;
	ammo = 5;
	remain = ammo;
	reloadingTime = 10.0f;
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

	if (remain <= 0 || isCoolDown)
	{
		return false;
	}
	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_SniperRifle::CoolComplete, 0.1f, true, 0);

	//���콺 Ʈ���̽��� ���� �÷��̾� ��Ʈ�ѷ�
	APlayerController* AimController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//���콺�� AimLayer�� �浹�� ��ġ
	FHitResult result;
	AimController->GetHitResultUnderCursorForObjects(objTypes, true, result);

	//�ѱ� Location
	FTransform temp = sniperMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//�ѱ��� �� ������ Location
	FVector temp2 = FVector(-140, temp.GetLocation().Y, temp.GetLocation().Z);
	//AimLayer = �÷��̾� X ��ġ �ݿ� �ȵż� -140 �ϵ��ڵ�, Y, Z�� ���콺 ����
	FVector dir = FVector(-140, result.Location.Y, result.Location.Z);

	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	params.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(hitInfo, temp2, FVector(dir - temp2) * 300000, ECollisionChannel::ECC_Visibility, params))
	{
		FTransform impactTransform(hitInfo.ImpactPoint);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, impactTransform);

		auto hitActor = hitInfo.GetActor();
		if (/*hitComp && hitComp->IsSimulatingPhysics()*/hitActor->IsA(AAssaultBoss::StaticClass()))
		{
			/*FVector dir = (hitInfo.ImpactPoint - start).GetSafeNormal();
			FVector force = dir * hitComp->GetMass() * 500;

			hitInfo.Component->AddForceAtLocation(force, dir);*/

			Cast<AAssaultBoss>(hitActor)->OnBossHit(10);
			UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
			Explosion();
		}
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, this->GetActorLocation(), 1, 1, 0);

	remain--;
	if (remain == 0)
	{
		GetWorldTimerManager().SetTimer(autoReloadTimerHandle, this, &AWeapon_SniperRifle::MagazineReloadComplete, 0.1f, true, 0);
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
	return remain;
}

FString AWeapon_SniperRifle::returnName()
{
	return myName;
}

void AWeapon_SniperRifle::MagazineReloadComplete()
{
	reloadingProgress += 0.1f;
	if (reloadingProgress >= reloadingTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sniper MagazineComplete"));
		remain = ammo;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), reloadSound, this->GetActorLocation(), 1, 1, 0);
		GetWorldTimerManager().ClearTimer(autoReloadTimerHandle);
		reloadingProgress = 0;
	}
}

void AWeapon_SniperRifle::CoolComplete()
{
	coolTimeProgress += 0.1f;
	if (coolTimeProgress >= coolTime)
	{
		isCoolDown = false;
		GetWorldTimerManager().ClearTimer(autoFireTimerHandle);
		coolTimeProgress = 0;
	}
}

void AWeapon_SniperRifle::Explosion()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, GetActorLocation());
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
}
