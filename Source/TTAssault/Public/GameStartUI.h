// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartUI.generated.h"

/**
 * 
 */
UCLASS()
class TTASSAULT_API UGameStartUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* GameStart_BackgroundImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn_Play;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Txt_Play;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Btn_GameSetting;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
		class UTextBlock* Txt_GameSetting;
};
