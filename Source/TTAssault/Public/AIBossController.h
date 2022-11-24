// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIBossController.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API AAIBossController : public AAIController
{
	GENERATED_BODY()

public:
	AAIBossController();
	virtual void OnPossess(APawn* InPawn) override;

	void RunAI();
	void StopAI();

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
