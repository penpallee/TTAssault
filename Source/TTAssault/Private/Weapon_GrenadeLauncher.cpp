// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_GrenadeLauncher.h"
#include "Bullet_Grenade.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"

AWeapon_GrenadeLauncher::AWeapon_GrenadeLauncher()
{
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("SkeletalMesh'/Game/TH/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGun.Object);
		gunMeshComp->SetVisibility(false);
	}
	//this->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
}

void AWeapon_GrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();
	Damage = 10;
	Cooltime = 0.7f;
	Ammo = 10;
	Remain = Ammo;
	reloadingTime = 2;
	isCoolDown = false;
	myName = TEXT("GrenadeLauncher");

	GetWorldTimerManager().SetTimer(autoReloadTimerHandle, this, &AWeapon_GrenadeLauncher::RemainReload, reloadingTime, true, 0);
}

void AWeapon_GrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

bool AWeapon_GrenadeLauncher::FireArm()
{
	//잔탄이 없거나 쿨타임 상태면 발사 실패
	if (Remain <= 0 || isCoolDown)
		return false;

	//쿨타임 설정 타이머
	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_GrenadeLauncher::CoolComplete, Cooltime, false);

	//마우스 트레이스를 위한 플레이어 컨트롤러
	APlayerController* AimController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//마우스가 AimLayer에 충돌한 위치
	FHitResult result;
	AimController->GetHitResultUnderCursorForObjects(objTypes, true, result);

	//총구 Location
	FTransform temp = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//총구의 맵 평면상의 Location
	FVector temp2 = FVector(-140, temp.GetLocation().Y, temp.GetLocation().Z);
	//AimLayer = 플레이어 X 위치 반영 안돼서 -140 하드코딩, Y, Z는 마우스 방향
	FVector dir = FVector(-140, result.Location.Y, result.Location.Z);

	//위에서 구한 location값들로 트랜스폼 생성
	FTransform aim = UKismetMathLibrary::MakeTransform(temp2, FRotator(UKismetMathLibrary::FindLookAtRotation(temp2, dir)), FVector(1));

	GetWorld()->SpawnActor<ABullet_Grenade>(bulletFactory, aim);

	//발사 효과음
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	//잔탄 감소
	Remain--;
	//쿨타임 상태
	isCoolDown = true;

	//발사 성공
	return true;
}

void AWeapon_GrenadeLauncher::OnSleep()
{
	gunMeshComp->SetVisibility(false);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon_GrenadeLauncher::OnAwake()
{
	gunMeshComp->SetVisibility(true);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

int AWeapon_GrenadeLauncher::returnAmmo()
{
	return Remain;
}

FString AWeapon_GrenadeLauncher::returnName()
{
	return myName;
}

void AWeapon_GrenadeLauncher::CoolComplete()
{
	isCoolDown=false;
}

void AWeapon_GrenadeLauncher::RemainReload()
{
	if(Remain<Ammo)
	Remain++;
}
