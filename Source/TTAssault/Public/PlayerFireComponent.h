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
};