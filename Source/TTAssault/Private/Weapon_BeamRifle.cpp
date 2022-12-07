// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_BeamRifle.h"
#include "Bullet_BeamRifle.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"

AWeapon_BeamRifle::AWeapon_BeamRifle()
{
	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gunMeshComp"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/TH/Resources/SniperGun/Sniper.Sniper'"));

	if (tempSniper.Succeeded())
	{
		sniperMeshComp->SetStaticMesh(tempSniper.Object);
		sniperMeshComp->SetRelativeScale3D(FVector(.15f));
		sniperMeshComp->SetVisibility(false);
	}
	//this->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
}

void AWeapon_BeamRifle::BeginPlay()
{
	Super::BeginPlay();
	damage = 10;
	coolTime = 1.5f;
	ammo = 20;
	remain = ammo;
	reloadingTime = 3;
	isCoolDown = false;
	myName = TEXT("BeamRifle");

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
	FTransform temp = sniperMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//총구의 맵 평면상의 Location
	FVector temp2 = FVector(0, temp.GetLocation().Y, temp.GetLocation().Z);
	//AimLayer = 플레이어 X 위치 반영 안돼서 0 하드코딩, Y, Z는 마우스 방향
	FVector dir = FVector(0, result.Location.Y, result.Location.Z);

	//위에서 구한 location값들로 트랜스폼 생성
	FTransform aim = UKismetMathLibrary::MakeTransform(temp2, FRotator(UKismetMathLibrary::FindLookAtRotation(temp2, dir)), FVector(1));

	GetWorld()->SpawnActor<ABullet_BeamRifle>(bulletFactory, aim);

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
	sniperMeshComp->SetVisibility(false);
	sniperMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon_BeamRifle::OnAwake()
{
	sniperMeshComp->SetVisibility(true);
	sniperMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
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
	reloadingProgress += 0.1f;
	if (remain < ammo && reloadingProgress >= reloadingTime)
	{
		remain++;
		reloadingProgress = 0;
	}
}