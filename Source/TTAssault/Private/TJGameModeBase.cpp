// Fill out your copyright notice in the Description page of Project Settings.


#include "TJGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameStartUI.h"

ATJGameModeBase::ATJGameModeBase()
{

}

void ATJGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::SetGamePaused(GetWorld(), true);

	GameStartUI = CreateWidget<UGameStartUI>(GetWorld(), GameStartUIF);

	GameStartUI->AddToViewport();
}
