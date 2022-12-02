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
	int Damage;					//���ݷ�
	UPROPERTY(EditAnywhere)
	float Cooltime;				//�߻� ������
	UPROPERTY(EditAnywhere)
	int Ammo;					//źâ ũ��
	UPROPERTY(EditAnywhere)
	float reloadingTime;		//������ �ð�

	int Remain;					//��ź��
	bool isMagazine;			//źâ�� ���� �Ǻ�
	bool isCoolDown;			//������ üũ
	FString myName;				//�����
	FTimerHandle autoFireTimerHandle;	//�߻� ������ üũ�� Ÿ�̸� �ڵ�
	FTimerHandle autoReloadTimerHandle;	//������ Ÿ�̸� �ڵ�

	virtual bool FireArm();		//���� �߻�
	virtual void OnSleep();		//��Ȱ��ȭ
	virtual void OnAwake();		//Ȱ��ȭ
	virtual int returnAmmo();	//��ź�� Ȯ�ο�
	virtual FString returnName();	//���� �̸� Ȯ�ο�
	virtual void AutoFire();	//������ ����
	virtual void MagazineReloadComplete();	//źâ ��ü �Ϸ�
	virtual void RemainReload();	//��źâ ������ ��ź ����
	virtual void CoolComplete();	//�߻� ������ ����

	UPROPERTY(EditAnywhere)
	USoundBase* fireSound;
	UPROPERTY(EditAnywhere)
	USoundBase* reloadSound;

	TArray<TEnumAsByte<EObjectTypeQuery> > objTypes;
};
