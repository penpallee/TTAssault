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
	//��ź�� ���ų� ��Ÿ�� ���¸� �߻� ����
	if (remain <= 0 || isCoolDown)
		return false;

	//��Ÿ�� ���� Ÿ�̸�
	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_BeamRifle::CoolComplete, 0.1f, true, 0);

	//���콺 Ʈ���̽��� ���� �÷��̾� ��Ʈ�ѷ�
	APlayerController* AimController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//���콺�� AimLayer�� �浹�� ��ġ
	FHitResult result;
	AimController->GetHitResultUnderCursorForObjects(objTypes, true, result);

	//�ѱ� Location
	FTransform temp = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//�ѱ��� �� ������ Location
	FVector temp2 = FVector(0, temp.GetLocation().Y, temp.GetLocation().Z);
	//AimLayer = �÷��̾� X ��ġ �ݿ� �ȵż� 0 �ϵ��ڵ�, Y, Z�� ���콺 ����
	FVector dir = FVector(0, result.Location.Y, result.Location.Z);

	//������ ���� location����� Ʈ������ ����
	FTransform aim = UKismetMathLibrary::MakeTransform(temp2, FRotator(UKismetMathLibrary::FindLookAtRotation(temp2, dir)), FVector(1));

	//GetWorld()->SpawnActor<ABullet_Grenade>(bulletFactory, aim);

	//�߻� ȿ����
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	//��ź ����
	remain--;
	//��Ÿ�� ����
	isCoolDown = true;

	//�߻� ����
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