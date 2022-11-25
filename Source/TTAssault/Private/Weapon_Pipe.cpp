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
		//meleeMeshComp->SetRelativeRotation(FRotator(90, 0, 0));
		meleeMeshComp->SetRelativeLocation(FVector(-60, 0, 0));
		meleeMeshComp->SetRelativeScale3D(FVector(.15f));
	}
}

// Called when the game starts
void AWeapon_Pipe::BeginPlay()
{
	Super::BeginPlay();
	Damage=40;
	Ammo = 2147483646;
	Cooltime=0.5f;
}

void AWeapon_Pipe::FireArm()
{
	UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
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