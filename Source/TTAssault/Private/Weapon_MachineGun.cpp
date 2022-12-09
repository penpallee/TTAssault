// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_MachineGun.h"
#include "Bullet_MachineGun.h"
#include <Kismet/GameplayStatics.h>

#include "Kismet/KismetMathLibrary.h"

AWeapon_MachineGun::AWeapon_MachineGun()
{
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGun(TEXT("SkeletalMesh'/Game/TH/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGun.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGun.Object);
		gunMeshComp->SetVisibility(false);
	}
	//this->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));

	bReplicates = true;
}

void AWeapon_MachineGun::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon_MachineGun, machineOwner);
}

void AWeapon_MachineGun::BeginPlay()
{
	Super::BeginPlay();
	damage = 1;
	coolTime = 0.03f;
	ammo = 30;
	remain = ammo;
	reloadingTime = 3;
	isCoolDown = false;
	myName = TEXT("MachineGun");
}

void AWeapon_MachineGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWeapon_MachineGun::FireArm/*_Implementation*/()
{
	//Super::FireArm();
	if (isCoolDown || remain <= 0)
		return false;

	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_MachineGun::AutoFire, coolTime, true, 0);

	return true;
}

void AWeapon_MachineGun::FireStop_Implementation()
{
	GetWorldTimerManager().ClearTimer(autoFireTimerHandle);
}

void AWeapon_MachineGun::OnSleep_Implementation()
{
	gunMeshComp->SetVisibility(false);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon_MachineGun::OnAwake_Implementation()
{
	gunMeshComp->SetVisibility(true);
	gunMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

int AWeapon_MachineGun::returnAmmo()
{
	return remain;
}

FString AWeapon_MachineGun::returnName()
{
	return myName;
}

void AWeapon_MachineGun::CoolComplete()
{
	isCoolDown = false;
}

void AWeapon_MachineGun::MagazineReloadComplete()
{
	reloadingProgress += 0.1f;
	if (reloadingProgress >= reloadingTime)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), reloadSound);
		remain = ammo;
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), reloadSound, this->GetActorLocation(), 1, 1, 0);
		GetWorldTimerManager().ClearTimer(autoReloadTimerHandle);
		reloadingProgress = 0;
	}
}

void AWeapon_MachineGun::AutoFire_Implementation()
{
	if (remain < 0)
	{
		GetWorldTimerManager().SetTimer(autoReloadTimerHandle, this, &AWeapon_MachineGun::MagazineReloadComplete, 0.1f, true, 0);
		return;
	}
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);

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

	GetWorld()->SpawnActor<ABullet_MachineGun>(bulletFactory, aim);
	remain--;
	//PlayAttackAnim();
}