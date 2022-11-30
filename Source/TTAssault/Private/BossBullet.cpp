// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBullet.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossBullet::ABossBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABossBullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle GravityTimerHandle;
	float GravityTime = 4;
	
	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			this->Destroy();
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionFX, GetActorLocation());

			// TimerHandle 초기화
			GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);	// 반복하려면 false를 true로 변경
}

// Called every frame
void ABossBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

