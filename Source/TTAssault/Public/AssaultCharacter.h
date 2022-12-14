// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "AssaultCharacter.generated.h"

UENUM(BlueprintType)
enum class WeaponSel : uint8 {
	Primary UMETA(displayname=PRIMARY),
	Secondary UMETA(displayname = SECONDARY),
	Tertiary UMETA(displayname = TERTIARY)
};

USTRUCT(Atomic, BlueprintType)
struct FPlayerStatus
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	int HPMax;
	UPROPERTY(BlueprintReadWrite)
	int HP;
	UPROPERTY(BlueprintReadWrite)
	float boostMax;
	UPROPERTY(BlueprintReadWrite)
	float boost;
	UPROPERTY(BlueprintReadWrite)
	int ammo;
	UPROPERTY(BlueprintReadWrite)
	int magazine;
	UPROPERTY(BlueprintReadWrite)
	FString WeaponName;
	UPROPERTY(BlueprintReadWrite)
	float coolTime;
	UPROPERTY(BlueprintReadWrite)
	float coolTimeProgress;
	UPROPERTY(BlueprintReadWrite)
	float reloadTime;
	UPROPERTY(BlueprintReadWrite)
	float reloadProgress;
};

UCLASS()
class TTASSAULT_API AAssaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAssaultCharacter();
	UPROPERTY(replicated)
		AAssaultCharacter* myOwner;
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty> &OutLifetimeProps) const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnCapsuleComponentBeginOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCameraComponent* cameraComp;	

	virtual void OnAxisHorizontal(float value);
	virtual void OnAxisVertical(float value);
	virtual void onAxisMouseX(float value);
	virtual void onAxisMouseY(float value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FVector direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FRotator rotation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float boosterMax = 1000;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float booster = boosterMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float boosterLose=100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HPMax = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int HP = HPMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isBooster = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Defense;

	//UPROPERTY(ReplicatedUsing = Tick)
	float mouseX;
	//UPROPERTY(ReplicatedUsing = Tick)
	float mouseY;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Replicated)
	class USkeletalMeshComponent* bodyMeshComp;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* attackAnimMontage;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* dieExplosionFactory;

	UPROPERTY(EditAnywhere)
	class UUserWidget* GameOverUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> GameOverUIFactory;

	WeaponSel selWeapon;
	FTimerHandle autoBoosterTimerHandle;	//?????? ?????? ????

	virtual void OnPlayerHit(int damage);
	virtual void OnPlayerDie();
	virtual void Stop();
	//UFUNCTION(NetMulticast, reliable)
	virtual void onSelPrimary();
	virtual void onSelSecondary();
	virtual void onSelTetertiary();
	virtual void onActionBoost();
	virtual void OnActionFire();
	virtual void OnActionStop();
	virtual FPlayerStatus returnStatus();
	virtual void PlayAttackAnim();
	virtual void boosterCharge();
};
