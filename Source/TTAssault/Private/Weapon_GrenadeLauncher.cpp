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
	//��ź�� ���ų� ��Ÿ�� ���¸� �߻� ����
	if (Remain <= 0 || isCoolDown)
		return false;

	//��Ÿ�� ���� Ÿ�̸�
	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_GrenadeLauncher::CoolComplete, Cooltime, false);

	//���콺 Ʈ���̽��� ���� �÷��̾� ��Ʈ�ѷ�
	APlayerController* AimController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//���콺�� AimLayer�� �浹�� ��ġ
	FHitResult result;
	AimController->GetHitResultUnderCursorForObjects(objTypes, true, result);

	//�ѱ� Location
	FTransform temp = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//�ѱ��� �� ������ Location
	FVector temp2 = FVector(-140, temp.GetLocation().Y, temp.GetLocation().Z);
	//AimLayer = �÷��̾� X ��ġ �ݿ� �ȵż� -140 �ϵ��ڵ�, Y, Z�� ���콺 ����
	FVector dir = FVector(-140, result.Location.Y, result.Location.Z);

	//������ ���� location����� Ʈ������ ����
	FTransform aim = UKismetMathLibrary::MakeTransform(temp2, FRotator(UKismetMathLibrary::FindLookAtRotation(temp2, dir)), FVector(1));

	GetWorld()->SpawnActor<ABullet_Grenade>(bulletFactory, aim);

	//�߻� ȿ����
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	//��ź ����
	Remain--;
	//��Ÿ�� ����
	isCoolDown = true;

	//�߻� ����
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
