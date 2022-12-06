// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Pipe.h"
#include <Kismet/GameplayStatics.h>

AWeapon_Pipe::AWeapon_Pipe()
{
	meleeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meleeMeshComp"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMelee(TEXT("StaticMesh'/Game/StarterContent/Props/SM_PillarFrame.SM_PillarFrame'"));

	if (tempMelee.Succeeded())
	{
		meleeMeshComp->SetStaticMesh(tempMelee.Object);
		meleeMeshComp->SetRelativeLocation(FVector(-60, 0, 0));
		meleeMeshComp->SetRelativeScale3D(FVector(.5f));
		meleeMeshComp->SetRelativeRotation(FRotator(90, 90, 90));
	}

	//this->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_rSocket"));
}

// Called when the game starts
void AWeapon_Pipe::BeginPlay()
{
	Super::BeginPlay();
	damage = 30;
	ammo = 2147483646;
	coolTime = 0.7f;
	comboMax = 3;
	comboTime = 0.5f;
	remain = ammo;
	isCoolDown = false;
	myName = TEXT("TitaniumPipe");
}

void AWeapon_Pipe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (coolTimeProgress > comboTime)
	{
		UE_LOG(LogTemp,Warning,TEXT("ÄðÃÊ"));
		combo = 1;
	}
}

bool AWeapon_Pipe::FireArm()
{
	if (isCoolDown)
		return false;
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);

	GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &AWeapon_Pipe::CoolComplete, 0.1f, true, 0);
	
	isCoolDown = true;
	return true;
}

void AWeapon_Pipe::OnSleep()
{
	meleeMeshComp->SetVisibility(false);
	meleeMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
void AWeapon_Pipe::OnAwake()
{
	meleeMeshComp->SetVisibility(true);
	meleeMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

int AWeapon_Pipe::returnAmmo()
{
	return remain;
}

FString AWeapon_Pipe::returnName()
{
	return myName;
}

void AWeapon_Pipe::CoolComplete()
{
	coolTimeProgress += 0.1f;
	if (coolTimeProgress >= coolTime)
	{
		isCoolDown = false;
		GetWorldTimerManager().ClearTimer(autoFireTimerHandle);
		coolTimeProgress = 0;
	}
}