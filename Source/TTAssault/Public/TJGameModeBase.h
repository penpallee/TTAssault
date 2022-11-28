// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TJGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API ATJGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATJGameModeBase();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGameStartUI> GameStartUIF;
	UPROPERTY(EditAnywhere)
	class UGameStartUI* GameStartUI;
};
