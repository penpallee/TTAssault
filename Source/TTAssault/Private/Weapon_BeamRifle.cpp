// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_BeamRifle.h"

#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"

AWeapon_BeamRifle::AWeapon_BeamRifle()
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

void AWeapon_BeamRifle::BeginPlay()
{
	Super::BeginPlay();
	damage = 10;
	coolTime = 0.7f;
	ammo = 10;
	remain = ammo;
	reloadingTime = 2;
	isCoolDown = false;
	myName = TEXT("GrenadeLauncher");

	GetWorldTimerManager().SetTimer(autoReloadTimerHandle, this, &AWeapon_BeamRifle::RemainReload, 0.1f, true, 0);
}

void AWeapon_BeamRifle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWeapon_BeamRifle::FireArm()
{
	//잔탄이 없거나 쿨타임 상태면 발사 실패
	if (remain <= 0 || isCoolDown)
		return false;

	//쿨타임 설정 타이머
	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_BeamRifle::CoolComplete, 0.1f, true, 0);

	//마우스 트레이스를 위한 플레이어 컨트롤러
	APlayerController* AimController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//마우스가 AimLayer에 충돌한 위치
	FHitResult result;
	AimController->GetHitResultUnderCursorForObjects(objTypes, true, result);

	//총구 Location
	FTransform temp = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//총구의 맵 평면상의 Location
	FVector temp2 = FVector(0, temp.GetLocation().Y, temp.GetLocation().Z);
	//AimLayer = 플레이어 X 위치 반영 안돼서 0 하드코딩, Y, Z는 마우스 방향
	FVector dir = FVector(0, result.Location.Y, result.Location.Z);

	//위에서 구한 location값들로 트랜스폼 생성
	FTransform aim = UKismetMathLibrary::MakeTransform(temp2, FRotator(UKismetMathLibrary::FindLookAtRotation(temp2, dir)), FVector(1));

	//GetWorld()->SpawnActor<ABullet_Grenade>(bulletFactory, aim);

	//발사 효과음
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	//잔탄 감소
	remain--;
	//쿨타임 상태
	isCoolDown = true;

	//발사 성공
	return true;
}

void AWeapon_BeamRifle::OnSleep()
{
	gunMeshComp->SetVisibility(false);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon_BeamRifle::OnAwake()
{
	gunMeshComp->SetVisibility(true);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

int AWeapon_BeamRifle::returnAmmo()
{
	return remain;
}

FString AWeapon_BeamRifle::returnName()
{
	return myName;
}

void AWeapon_BeamRifle::CoolComplete()
{
	//UE_LOG(LogTemp, Warning, TEXT("Grenade cool...%f"), coolTimeProgress);
	coolTimeProgress += 0.1f;
	if (coolTimeProgress >= coolTime)
	{
		isCoolDown = false;
		coolTimeProgress = 0;
		GetWorldTimerManager().ClearTimer(autoFireTimerHandle);
	}
}

void AWeapon_BeamRifle::RemainReload()
{
	//UE_LOG(LogTemp, Warning, TEXT("grenade reload...%f"), reloadingProgress);
	reloadingProgress += 0.1f;
	if (remain < ammo && reloadingProgress >= reloadingTime)
	{
		remain++;
		reloadingProgress = 0;
	}
}