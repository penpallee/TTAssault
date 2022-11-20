// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AssaultCharacter.h"
#include "PlayerFireComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerFireComponent : public UActorComponent
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

	//UPROPERTY(EditAnywhere)
	//class UStaticMeshComponent* meshComp;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class ABulletActor> bulletFactory;

	class AAssaultCharacter* myOwner;//전방선언, include what you use

	UPROPERTY(EditAnywhere)
	USoundBase* fireSound;
};