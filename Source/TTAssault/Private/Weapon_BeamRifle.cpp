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
	FTransform temp = sniperMeshComp->GetSocketTransform(TEXT("FirePosition"));
	//�ѱ��� �� ������ Location
	FVector temp2 = FVector(0, temp.GetLocation().Y, temp.GetLocation().Z);
	//AimLayer = �÷��̾� X ��ġ �ݿ� �ȵż� 0 �ϵ��ڵ�, Y, Z�� ���콺 ����
	FVector dir = FVector(0, result.Location.Y, result.Location.Z);

	//������ ���� location����� Ʈ������ ����
	FTransform aim = UKismetMathLibrary::MakeTransform(temp2, FRotator(UKismetMathLibrary::FindLookAtRotation(temp2, dir)), FVector(1));

	GetWorld()->SpawnActor<ABullet_BeamRifle>(bulletFactory, aim);

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