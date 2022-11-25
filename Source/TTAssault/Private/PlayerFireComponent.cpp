// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComponent.h"
#include "AssaultCharacter.h"
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Actor.h>

// Sets default values for this component's properties
UPlayerFireComponent::UPlayerFireComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPlayerFireComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPlayerFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}