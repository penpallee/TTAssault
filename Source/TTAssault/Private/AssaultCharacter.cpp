// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/WidgetLayoutLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

#include "Blueprint/UserWidget.h"

// Sets default values
AAssaultCharacter::AAssaultCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn'"));//스켈레탈 메시 가져옴
	//if (tempBody.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(tempBody.Object);
	//	bodyMeshComp=GetMesh();
	//	bodyMeshComp->SetRelativeLocation(FVector(0, 0, -90));
	//}

	//this->SetActorRotation(FRotator(0, 0, -90));

	//springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	//springArmComp->SetupAttachment((RootComponent));
	//springArmComp->SetRelativeLocation(FVector(0, -90, 0));
	//springArmComp->TargetArmLength = 1000;

	//cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	//cameraComp->SetupAttachment(springArmComp);

	//	weaponComponent = CreateDefaultSubobject <UPlayerFireComponent>("fireComp");//모듈화
	//	weaponComponent->SetupAttachment(this->GetMesh());

	bReplicates = true;
}

void AAssaultCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AAssaultCharacter, myOwner);
	DOREPLIFETIME(AAssaultCharacter, direction);
	DOREPLIFETIME(AAssaultCharacter, rotation);

}

// Called when the game starts or when spawned
void AAssaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	/*InputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AAssaultCharacter::onActionBoost);
	InputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &AAssaultCharacter::OnAxisHorizontal);
	InputComponent->BindAxis(TEXT("Move Right / Left"), this, &AAssaultCharacter::OnAxisVertical);
	InputComponent->BindAxis("Look Up / Down Mouse", this, &AAssaultCharacter::onAxisMouseY);
	InputComponent->BindAxis("Turn Right / Left Mouse", this, &AAssaultCharacter::onAxisMouseX);
	speed = 500;
	booster = 1000;
	UGameplayStatics::GetPlayerController(this, 0)->bShowMouseCursor = true;*/
}

// Called every frame
void AAssaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*this->SetActorLocation(GetActorLocation() + direction * speed * DeltaTime);*/
}

// Called to bind functionality to input
void AAssaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//weaponComponent->SetupFire(PlayerInputComponent);
}

void AAssaultCharacter::OnCapsuleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void AAssaultCharacter::OnAxisHorizontal(float value)
{
	/*direction.Y = -value;*/
}

void AAssaultCharacter::OnAxisVertical(float value)
{
	/*direction.X = value;*/
}

void AAssaultCharacter::onAxisMouseX(float value)
{

}

void AAssaultCharacter::onAxisMouseY(float value)
{

}

void AAssaultCharacter::onActionBoost()
{

}

void AAssaultCharacter::OnActionFire()
{

}

void AAssaultCharacter::OnActionStop()
{

}


FPlayerStatus AAssaultCharacter::returnStatus()
{
	FPlayerStatus nowStat;
	return nowStat;
}

void AAssaultCharacter::OnPlayerHit(int damage)
{
	HP -= (damage - Defense);
	if (HP <= 0)
	{
		OnPlayerDie();
	}
}

void AAssaultCharacter::OnPlayerDie()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), dieExplosionFactory, this->GetActorLocation());
	Destroy();

	GameOverUI = CreateWidget<UUserWidget>(GetWorld(), GameOverUIFactory);

	FTimerHandle GravityTimerHandle;
	float GravityTime = 2;

	GetWorld()->GetTimerManager().SetTimer(GravityTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			// 코드 구현
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			GameOverUI->AddToViewport();
			// TimerHandle 초기화
			//GetWorld()->GetTimerManager().ClearTimer(GravityTimerHandle);
		}), GravityTime, false);	// 반복하려면 false를 true로 변경
}


void AAssaultCharacter::Stop()
{

}


void AAssaultCharacter::onSelPrimary()
{

}
void AAssaultCharacter::onSelSecondary()
{

}
void AAssaultCharacter::onSelTetertiary()
{

}

void AAssaultCharacter::PlayAttackAnim()
{
	PlayAnimMontage(attackAnimMontage);
}

void AAssaultCharacter::boosterCharge()
{

}
