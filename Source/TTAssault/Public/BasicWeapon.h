// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicWeapon.generated.h"

UCLASS()
class TTASSAULT_API ABasicWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int damage;					//공격력
	UPROPERTY(EditAnywhere)
	float coolTime;				//발사 딜레이
	float coolTimeProgress;		//쿨타임 진행률
	UPROPERTY(EditAnywhere)
	int ammo;					//탄창 크기
	UPROPERTY(EditAnywhere)
	float reloadingTime;		//재장전 시간
	float reloadingProgress;	//재장전률

	int remain;					//잔탄량
	bool isMagazine;			//탄창형 무기 판별
	bool isCoolDown;			//딜레이 체크
	FString myName;				//무기명
	FTimerHandle autoFireTimerHandle;	//발사 딜레이 체크용 타이머 핸들
	FTimerHandle autoReloadTimerHandle;	//재장전 타이머 핸들

	virtual bool FireArm();		//무기 발사
	virtual void OnSleep();		//비활성화
	virtual void OnAwake();		//활성화
	virtual int returnAmmo();	//잔탄량 확인용
	virtual int returnMagazine(); //전체 탄창 크기
	virtual FString returnName();	//무기 이름 확인용
	virtual float returnCoolTime();
	virtual float returnCoolProgress();
	virtual float returnReloadTime();
	virtual float returnReloadProgress();
	virtual void AutoFire();	//연사형 무기
	virtual void MagazineReloadComplete();	//탄창 교체 완료
	virtual void RemainReload();	//비탄창 무기의 잔탄 장전
	virtual void CoolComplete();	//발사 딜레이 종료

	UPROPERTY(EditAnywhere)
	USoundBase* fireSound;
	UPROPERTY(EditAnywhere)
	USoundBase* reloadSound;

	TArray<TEnumAsByte<EObjectTypeQuery> > objTypes;
};
