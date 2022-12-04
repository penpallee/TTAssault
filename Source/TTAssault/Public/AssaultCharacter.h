// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
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
	int HP;
	UPROPERTY(BlueprintReadWrite)
	float boost;
	UPROPERTY(BlueprintReadWrite)
	int ammo;
	UPROPERTY(BlueprintReadWrite)
	FString WeaponName;
};

UCLASS()
class TTASSAULT_API AAssaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAssaultCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UCameraComponent* cameraComp;	

	virtual void OnAxisHorizontal(float value);
	virtual void OnAxisVertical(float value);
	virtual void onAxisMouseX(float value);
	virtual void onAxisMouseY(float value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector direction;
	FRotator rotation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float speed;
	UPROPERTY(EditAnywhere)
	float booster = 1000;
	UPROPERTY(EditAnywhere)
	int HP = 100;
	UPROPERTY(EditAnywhere)
	bool isBooster = false;
	UPROPERTY(EditAnywhere)
	int Defense;

	float mouseX;
	float mouseY;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* bodyMeshComp;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* attackAnimMontage;

	WeaponSel selWeapon;

	virtual void OnPlayerHit(int damage);
	virtual void Stop();
	virtual void onSelPrimary();
	virtual void onSelSecondary();
	virtual void onSelTetertiary();
	virtual void onActionBoost();
	virtual void OnActionFire();
	virtual FPlayerStatus returnStatus();
	virtual void PlayAttackAnim();
};
