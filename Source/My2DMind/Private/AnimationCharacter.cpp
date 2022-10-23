// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "EnemyCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

AAnimationCharacter::AAnimationCharacter()
{

	bFootStepGate = true;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->SetRelativeRotation(FRotator(-20.f, 0, 0));
	SpringArmComp->TargetOffset = FVector(0, 0, 200);
	SpringArmComp->bDoCollisionTest = false;
	SpringArmComp->TargetArmLength = 2500.f;
	SpringArmComp->bInheritPitch = true;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bInheritYaw = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->bEnableCameraRotationLag = true;
	SpringArmComp->CameraLagSpeed = 4.f;
	SpringArmComp->CameraRotationLagSpeed = 8.f;
	SpringArmComp->CameraLagMaxDistance = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->FieldOfView = 55.f;
	CameraComp->PostProcessBlendWeight = 1.f;
	CameraComp->PostProcessSettings.bOverride_DepthOfFieldFstop = true;
	CameraComp->PostProcessSettings.DepthOfFieldFstop = 0.5f;
	CameraComp->PostProcessSettings.bOverride_DepthOfFieldSensorWidth = true;
	CameraComp->PostProcessSettings.DepthOfFieldSensorWidth = 100.f;
	CameraComp->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	CameraComp->PostProcessSettings.DepthOfFieldFocalDistance = SpringArmComp->TargetArmLength;
	CameraComp->PostProcessSettings.bOverride_DepthOfFieldMinFstop = true;
	CameraComp->PostProcessSettings.DepthOfFieldMinFstop = 0.f;
	CameraComp->PostProcessSettings.MotionBlurAmount = 0.f;

	GetCapsuleComponent()->SetCapsuleRadius(70.f);

	GetSprite()->SetRelativeScale3D(FVector(11.f,11.f,11.f));
	GetSprite()->SetUsingAbsoluteRotation(true);
	GetSprite()->SetFlipbook(FlipBooks.IdleDown);
	GetSprite()->CastShadow = true;

	bIsMoving = false;


	GetCharacterMovement()->GravityScale = 2.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;

	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(RootComponent);
	AttackSphere->SetSphereRadius(attackRadius);
	bAttackGate = true;
}

void AAnimationCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnCharacterMovementUpdated.AddDynamic(this, &AAnimationCharacter::Animate);
}

void AAnimationCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue /* = 1.0f */, bool bForce /* = false */)
{
	UPawnMovementComponent* movementComp = GetMovementComponent();

	if (movementComp)
	{
		movementComp->AddInputVector(WorldDirection * ScaleValue, bForce);
	}
}

void AAnimationCharacter::Animate(float DeltaTime, FVector oldLoc, FVector oldVelocity)
{
	if (!bAttackGate) return;

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
		case EAnimationDirection::Up:
			GetSprite()->SetFlipbook(FlipBooks.WalkUp);
			break;
		case EAnimationDirection::Down:
			GetSprite()->SetFlipbook(FlipBooks.WalkDown);
			break;
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(FlipBooks.WalkLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(FlipBooks.WalkRight);
			break;
		case EAnimationDirection::UpLeft:
			GetSprite()->SetFlipbook(FlipBooks.WalkUpLeft);
			break;
		case EAnimationDirection::UpRight:
			GetSprite()->SetFlipbook(FlipBooks.WalkUpRight);
			break;
		case EAnimationDirection::DownLeft:
			GetSprite()->SetFlipbook(FlipBooks.WalkDownLeft);
			break;
		case EAnimationDirection::DownRight:
			GetSprite()->SetFlipbook(FlipBooks.WalkDownRight);
			break;
		default:
			break;
		}

		if (!GetCharacterMovement()->IsFalling())
		{
			if (bFootStepGate)
			{
				bFootStepGate = !bFootStepGate;
				OnFootstep();
				GetWorldTimerManager().SetTimer(FootStepTimer, [this]() {bFootStepGate = true;}, 0.3f, false);
			}
		}
	}
	else
	{
		switch (currentAnimDirection)
		{
		case EAnimationDirection::Up:
			GetSprite()->SetFlipbook(FlipBooks.IdleUp);
			break;
		case EAnimationDirection::Down:
			GetSprite()->SetFlipbook(FlipBooks.IdleDown);
			break;
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(FlipBooks.IdleLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(FlipBooks.IdleRight);
			break;
		case EAnimationDirection::UpLeft:
			GetSprite()->SetFlipbook(FlipBooks.IdleUpLeft);
			break;
		case EAnimationDirection::UpRight:
			GetSprite()->SetFlipbook(FlipBooks.IdleUpRight);
			break;
		case EAnimationDirection::DownLeft:
			GetSprite()->SetFlipbook(FlipBooks.IdleDownLeft);
			break;
		case EAnimationDirection::DownRight:
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


void AAnimationCharacter::SetCurrentAnimDirection(const FVector& Velocity, TOptional<FMinimalViewInfo> ViewInfo)
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
			currentAnimDirection = EAnimationDirection::Up;
		}
		else if (ForwardSpeed > 0.5f && RightSpeed >= 0.5f)
		{
			currentAnimDirection = EAnimationDirection::UpRight;
		}
		else if (ForwardSpeed > 0.5f && RightSpeed <= -0.5f)
		{
			currentAnimDirection = EAnimationDirection::UpLeft;
		}
		else if (ForwardSpeed < 0.5f && abs(RightSpeed) <= 0.5f)
		{
			currentAnimDirection = EAnimationDirection::Down;
		}
		else if (ForwardSpeed < -0.5f && RightSpeed >= 0.5f)
		{
			currentAnimDirection = EAnimationDirection::DownRight;
		}
		else if (ForwardSpeed < -0.5f && RightSpeed <= -0.5f)
		{
			currentAnimDirection = EAnimationDirection::DownLeft;
		}
		else if (abs(ForwardSpeed) < 0.5f && RightSpeed > 0.0f)
		{
			currentAnimDirection = EAnimationDirection::Right;
		}
		else
		{
			currentAnimDirection = EAnimationDirection::Left;
		}
	}
}

void AAnimationCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Up"), this, &AAnimationCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("Right"), this, &AAnimationCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AAnimationCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AAnimationCharacter::Turn);

}

void AAnimationCharacter::MoveForward(float Value)
{
	if (Value != 0.f)
	{
		const FVector direction = CameraComp->GetForwardVector();
		AddMovementInput(direction, Value, false);
	}
}
void AAnimationCharacter::MoveRight(float Value)
{
	if (Value != 0.f)
	{
		const FVector direction = CameraComp->GetRightVector();
		AddMovementInput(direction, Value, false);
	}
}

void AAnimationCharacter::LookUp(float Value)
{
	if (Value != 0.f)
	{
		AddControllerPitchInput(Value);
	}
}
void AAnimationCharacter::Turn(float Value)
{
	if (Value != 0.f)
	{
		AddControllerYawInput(Value);
	}
}

void AAnimationCharacter::OnJumped_Implementation()
{
	GetCharacterMovement()->bNotifyApex = true;

	JumpCounter++;
	GetWorldTimerManager().ClearTimer(JumpReset);

	Super::OnJumped_Implementation();
}

void AAnimationCharacter::Landed(const FHitResult& Hit)
{
	GetSprite()->SetPlayRate(1);
	GetCharacterMovement()->GravityScale = 2.8f;

	if (JumpCounter > 2)
		ResetJumpPower();

	ModifyJumpPower();
	GetWorldTimerManager().SetTimer(JumpReset, this, &AAnimationCharacter::ResetJumpPower, 0.2f, false);

	Super::Landed(Hit);
}

void AAnimationCharacter::NotifyJumpApex()
{
	GetCharacterMovement()->GravityScale = 5.f;
	Super::NotifyJumpApex();
}

void AAnimationCharacter::ResetJumpPower()
{
	JumpCounter = 0;
	GetCharacterMovement()->JumpZVelocity = 1000;
}

bool AAnimationCharacter::Punch(UNiagaraSystem* SystemToSpawn)
{
	bool bHit{ false };
	FVector NewLocation{ GetSprite()->GetRelativeLocation() };

	if (bAttackGate)
	{
		switch (currentAnimDirection)
		{
		case EAnimationDirection::Down:
			GetSprite()->SetFlipbook(PunchFlipBooks.PunchDown);
			NewLocation.X -= 25.f;
			GetSprite()->SetRelativeLocation(NewLocation);
			break;
		case EAnimationDirection::Up:
			GetSprite()->SetFlipbook(PunchFlipBooks.PunchUp);
			NewLocation.X += 25.f;
			GetSprite()->SetRelativeLocation(NewLocation);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(PunchFlipBooks.PunchRight);
			NewLocation.Y += 25.f;
			GetSprite()->SetRelativeLocation(NewLocation);
			break;
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(PunchFlipBooks.PunchLeft);
			NewLocation.Y -= 25.f;
			GetSprite()->SetRelativeLocation(NewLocation);
			break;
		case EAnimationDirection::DownRight:
			GetSprite()->SetFlipbook(PunchFlipBooks.PunchDownRight);
			NewLocation.X -= 25.f;
			NewLocation.Y += 25.f;
			GetSprite()->SetRelativeLocation(NewLocation);
			break;
		case EAnimationDirection::DownLeft:
			GetSprite()->SetFlipbook(PunchFlipBooks.PunchDownLeft);
			NewLocation.X -= 25.f;
			NewLocation.Y -= 25.f;
			GetSprite()->SetRelativeLocation(NewLocation);
			break;
		case EAnimationDirection::UpRight:
			GetSprite()->SetFlipbook(PunchFlipBooks.PunchUpRight);
			NewLocation.X += 25.f;
			NewLocation.Y += 25.f;
			GetSprite()->SetRelativeLocation(NewLocation);
			break;
		case EAnimationDirection::UpLeft:
			GetSprite()->SetFlipbook(PunchFlipBooks.PunchUpLeft);
			NewLocation.X += 25.f;
			NewLocation.Y -= 25.f;
			GetSprite()->SetRelativeLocation(NewLocation);
			break;
		default:
			break;
		}

		bAttackGate = false;
		GetWorldTimerManager().SetTimer(AttackTimer,
			[this]()
			{
				bAttackGate = true;
				GetSprite()->SetRelativeLocation(FVector::ZeroVector);
			},
			0.3f, false);

		TArray<AActor*> overlappingActors;
		AttackSphere->GetOverlappingActors(overlappingActors, AEnemyCharacter::StaticClass());
		if (overlappingActors.Num()>0)
		{
			bHit = true;
			for (AActor* Actor : overlappingActors)
			{
				AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Actor);
				const FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(
					GetActorLocation(), EnemyCharacter->GetActorLocation());
				EnemyCharacter->hp -= 25;
				EnemyCharacter->LaunchCharacter(FVector(Direction.X * attackForce, Direction.Y * attackForce , (Direction.Z +1)* attackForce),false,false);
				if (IsValid(SystemToSpawn))
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,
						SystemToSpawn,
						EnemyCharacter->GetActorLocation());
				}
			}
		}
	}
	return bHit;
}

void AAnimationCharacter::ModifyJumpPower()
{
	switch (JumpCounter)
	{
	case 1:
		GetCharacterMovement()->JumpZVelocity = 1400.f;
			break;
	case 2:
		GetCharacterMovement()->JumpZVelocity = 2000.f;
			break;
	default:
			break;
	}
}