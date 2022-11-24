// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
enum class WeponSel { 
	PRIMARY, SECONDARY, TERTIARY 
};

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AssaultCharacter.h"
#include "MyGrenade.h"
#include <Particles/ParticleSystem.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "PlayerFireComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerFireComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFireComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupFire(UInputComponent* PlayerInputComponent);
	void OnActionFire();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMyGrenade> bulletFactory;

	class AAssaultCharacter* myOwner;//전방선언, include what you use

	UPROPERTY(EditAnywhere)
	USoundBase* fireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UArrowComponent* FirePosition;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meleeMeshComp;
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* gunMeshComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* sniperMeshComp;
	UPROPERTY(EditAnywhere)
	class UParticleSystem* bulletImpactFactory;

	WeponSel selWeapon;
	//FTransform t;

	void onSelPrimary();
	void onSelSecondary();
	void onSelTetertiary();
	void SetGun(WeponSel num);
	void Sniping();
};