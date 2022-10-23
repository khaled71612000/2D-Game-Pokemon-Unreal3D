// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
AEnemyCharacter::AEnemyCharacter()
{
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GetCapsuleComponent()->SetCapsuleRadius(70.f);

	GetSprite()->SetRelativeScale3D(FVector(11.f, 11.f, 11.f));
	GetSprite()->SetUsingAbsoluteRotation(true);
	GetSprite()->SetFlipbook(FlipBooks.IdleDown);
	GetSprite()->CastShadow = true;

	bIsMoving = false;

	DeathCallBack.BindLambda([this](){OnSquashed();});
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnCharacterMovementUpdated.AddDynamic(this, &AEnemyCharacter::Animate);
}

void AEnemyCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue /* = 1.0f */, bool bForce /* = false */)
{
	UPawnMovementComponent* movementComp = GetMovementComponent();

	if (movementComp)
	{
		movementComp->AddInputVector(WorldDirection * ScaleValue, bForce);
	}
}

void AEnemyCharacter::Animate(float DeltaTime, FVector oldLoc, FVector oldVelocity)
{
	TOptional<FMinimalViewInfo> ViewInfo;
	if (!IsPlayerControlled())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			APlayerController* PlayerController = World->GetFirstPlayerController();
			if (PlayerController)
			{
				ACharacter* Character = PlayerController->GetCharacter();
				if (Character)
				{
					Character->CalcCamera(DeltaTime, ViewInfo.Emplace());
				}
			}
		}
	}

	SetCurrentAnimDirection(oldVelocity, ViewInfo);

	if (oldVelocity.Size() > 0.0f || GetCharacterMovement()->IsFalling())
	{
		switch (currentAnimDirection)
		{
		case EEnemyAnimationDirection::Up:
			GetSprite()->SetFlipbook(FlipBooks.WalkUp);
			break;
		case EEnemyAnimationDirection::Down:
			GetSprite()->SetFlipbook(FlipBooks.WalkDown);
			break;
		case EEnemyAnimationDirection::Left:
			GetSprite()->SetFlipbook(FlipBooks.WalkLeft);
			break;
		case EEnemyAnimationDirection::Right:
			GetSprite()->SetFlipbook(FlipBooks.WalkRight);
			break;
		case EEnemyAnimationDirection::UpLeft:
			GetSprite()->SetFlipbook(FlipBooks.WalkUpLeft);
			break;
		case EEnemyAnimationDirection::UpRight:
			GetSprite()->SetFlipbook(FlipBooks.WalkUpRight);
			break;
		case EEnemyAnimationDirection::DownLeft:
			GetSprite()->SetFlipbook(FlipBooks.WalkDownLeft);
			break;
		case EEnemyAnimationDirection::DownRight:
			GetSprite()->SetFlipbook(FlipBooks.WalkDownRight);
			break;
		default:
			break;
		}

	}
	else
	{
		switch (currentAnimDirection)
		{
		case EEnemyAnimationDirection::Up:
			GetSprite()->SetFlipbook(FlipBooks.IdleUp);
			break;
		case EEnemyAnimationDirection::Down:
			GetSprite()->SetFlipbook(FlipBooks.IdleDown);
			break;
		case EEnemyAnimationDirection::Left:
			GetSprite()->SetFlipbook(FlipBooks.IdleLeft);
			break;
		case EEnemyAnimationDirection::Right:
			GetSprite()->SetFlipbook(FlipBooks.IdleRight);
			break;
		case EEnemyAnimationDirection::UpLeft:
			GetSprite()->SetFlipbook(FlipBooks.IdleUpLeft);
			break;
		case EEnemyAnimationDirection::UpRight:
			GetSprite()->SetFlipbook(FlipBooks.IdleUpRight);
			break;
		case EEnemyAnimationDirection::DownLeft:
			GetSprite()->SetFlipbook(FlipBooks.IdleDownLeft);
			break;
		case EEnemyAnimationDirection::DownRight:
			GetSprite()->SetFlipbook(FlipBooks.IdleDownRight);
			break;
		default:
			break;
		}
	}


	if (GetCharacterMovement()->IsFalling())
	{
		GetSprite()->SetPlayRate(0.f);
		GetSprite()->SetPlaybackPositionInFrames(0, true);
	}
	else
	{
		GetSprite()->SetPlayRate(1.f);
	}
}

void AEnemyCharacter::SetCurrentAnimDirection(const FVector& Velocity, TOptional< FMinimalViewInfo> ViewInfo)
{
	FVector Forward;
	FVector Right;
	if (ViewInfo.IsSet())
	{
		Forward = UKismetMathLibrary::GetForwardVector(ViewInfo.GetValue().Rotation);
		Right = UKismetMathLibrary::GetRightVector(ViewInfo.GetValue().Rotation);
	}
	else
	{
		Forward = GetActorForwardVector().GetSafeNormal();
		Right = GetActorRightVector().GetSafeNormal();
	}

	const float ForwardSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Forward) * 100) / 100;
	const float RightSpeed = FMath::Floor(FVector::DotProduct(Velocity.GetSafeNormal(), Right) * 100) / 100;

	bIsMoving = ForwardSpeed != 0.0f || RightSpeed != 0.0f;

	if (bIsMoving && !GetCharacterMovement()->IsFalling())
	{
		if (ForwardSpeed > 0.0f && abs(RightSpeed) < 0.5f)
		{
			currentAnimDirection = EEnemyAnimationDirection::Up;
		}
		else if (ForwardSpeed > 0.5f && RightSpeed >= 0.5f)
		{
			currentAnimDirection = EEnemyAnimationDirection::UpRight;
		}
		else if (ForwardSpeed > 0.5f && RightSpeed <= -0.5f)
		{
			currentAnimDirection = EEnemyAnimationDirection::UpLeft;
		}
		else if (ForwardSpeed < 0.5f && abs(RightSpeed) <= 0.5f)
		{
			currentAnimDirection = EEnemyAnimationDirection::Down;
		}
		else if (ForwardSpeed < -0.5f && RightSpeed >= 0.5f)
		{
			currentAnimDirection = EEnemyAnimationDirection::DownRight;
		}
		else if (ForwardSpeed < -0.5f && RightSpeed <= -0.5f)
		{
			currentAnimDirection = EEnemyAnimationDirection::DownLeft;
		}
		else if (abs(ForwardSpeed) < 0.5f && RightSpeed > 0.0f)
		{
			currentAnimDirection = EEnemyAnimationDirection::Right;
		}
		else
		{
			currentAnimDirection = EEnemyAnimationDirection::Left;
		}
	}
}

bool AEnemyCharacter::isSquashHit(const FVector Velocity, const float VelocityToKill) const
{
	if ((Velocity.IsNormalized() ? Velocity.Z : Velocity.GetSafeNormal().Z) > VelocityToKill)
		return true;
	return false;
}

void AEnemyCharacter::Squash(const float TimeTellDestroy /*= 1.f*/)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (IsValid(Controller))
	{
		const auto AIController = Cast<AAIController>(Controller);
		if (AIController)
		{
			const auto Brain = AIController->GetBrainComponent();
			if (IsValid(Brain))
			{
				AIController->StopMovement();
				AIController->GetBrainComponent()->StopLogic(FString("AI IS SQUASHED"));
			}
		}
	}

	FVector newScale{ GetSprite()->GetRelativeScale3D() };
	newScale.Z /= 2.f;

	GetSprite()->SetRelativeScale3D(newScale);
	GetSprite()->AddLocalOffset(FVector(0,0,-55.f),false,nullptr,ETeleportType::TeleportPhysics);

	GetWorldTimerManager().SetTimer(DeathHandle, DeathCallBack, TimeTellDestroy, false);

}
