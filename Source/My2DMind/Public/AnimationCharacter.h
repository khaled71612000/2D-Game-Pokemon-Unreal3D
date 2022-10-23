// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
//#include "GameplayEffectTypes.h"
#include "Misc/Optional.h"
#include "Camera/CameraTypes.h"
#include "AbilitySystemInterface.h"
#include "AnimationCharacter.generated.h"
//
//
//class UMindGameplayAbility;
//class UGameplayEffect;
//class UMindAbilitySystemComponent;
//class UMindAttributeSet;

UENUM(BlueprintType)
enum class EAnimationDirection : uint8
{
	Down,
	Up,
	Right,
	Left,
	DownRight,
	DownLeft,
	UpRight,
	UpLeft
};

//UENUM(BlueprintType)
//enum class EAbilityInputID : uint8
//{
//	None,
//	Confirm,
//	Cancel,
//	Punch
//};

USTRUCT(BlueprintType)
struct MY2DMIND_API FPunchFlipbooks
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UPaperFlipbook> PunchDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UPaperFlipbook> PunchUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UPaperFlipbook> PunchRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UPaperFlipbook> PunchLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UPaperFlipbook> PunchDownRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UPaperFlipbook> PunchDownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UPaperFlipbook> PunchUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TObjectPtr<UPaperFlipbook> PunchUpLeft;
};

USTRUCT(BlueprintType)
struct MY2DMIND_API FMovementFlipbooks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> IdleDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> IdleUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> IdleRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> IdleLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> IdleDownRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> IdleDownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> IdleUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> IdleUpLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> WalkDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> WalkUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> WalkRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> WalkLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> WalkDownRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> WalkDownLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> WalkUpRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr <UPaperFlipbook> WalkUpLeft;
};

UCLASS()
class MY2DMIND_API AAnimationCharacter : public APaperCharacter/*, public IAbilitySystemInterface*/
{
	GENERATED_BODY()


public:

	AAnimationCharacter();

protected:

	virtual void BeginPlay() override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue /* = 1.0f */, bool bForce /* = false */) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnFootstep();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void Animate(
		float DeltaTime,
		FVector oldLoc,
		FVector oldVelocity);

	void SetCurrentAnimDirection(
		const FVector& Velocity,
		TOptional< FMinimalViewInfo> ViewInfo);

	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);
	
	void LookUp(float Value);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);

	virtual void OnJumped_Implementation() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void NotifyJumpApex() override;
	void ModifyJumpPower();
	void ResetJumpPower();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	EAnimationDirection currentAnimDirection {EAnimationDirection::Down};
	
	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = "Animation")
	FMovementFlipbooks FlipBooks;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	uint8 bIsMoving : 1;

	FTimerHandle FootStepTimer;
	uint8 bFootStepGate : 1;

	FTimerHandle JumpReset;
	int JumpCounter{};

	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool Punch(class UNiagaraSystem* SystemToSpawn);

	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = "Actions")
	FPunchFlipbooks PunchFlipBooks;
	FTimerHandle AttackTimer;
	uint8 bAttackGate : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr <class USphereComponent> AttackSphere;

	float attackForce{ 750.f };
	float attackRadius{ 150.f };

	//virtual void PossessedBy(AController* NewController) override;
	//virtual void OnRep_PlayerState() override;
	//virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	//TObjectPtr<UMindAbilitySystemComponent> AbilitySystemComponent;
	//UPROPERTY()
	//TObjectPtr<UMindAttributeSet> Attributes;
};
