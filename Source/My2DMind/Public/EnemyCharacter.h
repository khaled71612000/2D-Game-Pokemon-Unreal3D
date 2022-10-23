// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperFlipbook.h"
#include "Misc/Optional.h"
#include "Camera/CameraTypes.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyAnimationDirection : uint8
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
USTRUCT(BlueprintType)
struct MY2DMIND_API FEnemyMovementFlipbooks
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
class MY2DMIND_API AEnemyCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:

	AEnemyCharacter();
	UFUNCTION(BlueprintImplementableEvent, Category = "Components")
	void OnSquashed();

	int32 hp = 100;

protected:

	virtual void BeginPlay() override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue /* = 1.0f */, bool bForce /* = false */) override;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void Animate(
			float DeltaTime,
			FVector oldLoc,
			FVector oldVelocity);

	void SetCurrentAnimDirection(
		const FVector& Velocity,
		TOptional< FMinimalViewInfo> ViewInfo);

	UFUNCTION(BlueprintPure, Category = "Components")
	bool isSquashHit(const FVector Velocity, const float VelocityToKill = 0.f) const;
	
	UFUNCTION(BlueprintCallable, Category = "Components")
	void Squash(const float TimeTellDestroy = 1.f);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		EEnemyAnimationDirection currentAnimDirection {
		EEnemyAnimationDirection::Down
	};

	UPROPERTY(Editanywhere, BlueprintReadOnly, Category = "Animation")
		FEnemyMovementFlipbooks FlipBooks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
		uint8 bIsMoving : 1;

	FTimerDelegate DeathCallBack;
	FTimerHandle DeathHandle;
};
