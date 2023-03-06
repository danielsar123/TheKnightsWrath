// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacterAnimInstance.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"

UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
}

// Called every frame 
void UPlayerCharacterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (PlayerCharacter)
	{
		// Gets the character's state. The character state determines what animations are played for some actions
		CharacterState = PlayerCharacter->GetCharacterState();

		ActionState = PlayerCharacter->GetActionState();

		DeathPose = PlayerCharacter->GetDeathPose();

		// Get the speed of the character from velocity
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovementComponent->Velocity);

		// Is the character in the air?
		bIsInAir = PlayerMovementComponent->IsFalling();

		// Is the character accelerating?
		if (PlayerMovementComponent->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}

		FRotator AimRotation = PlayerCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(PlayerCharacter->GetVelocity());
		
		// Movement offset yaw gives us the difference between the player's movement rotation and aim rotation. With this information, we know what walking animation to play
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		// Gets the movement offset yaw last frame. Useful for playing the correct walking stop animations.
		if (PlayerCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		// Smoothly rotates character if they want to walk in the opposite direction they are facing
		SetActorRotToControlRot(GroundSpeed, DeltaTime, bIsAccelerating);

		InputForward = PlayerCharacter->GetMoveForwardValue();
		InputRight = PlayerCharacter->GetMoveRightValue();
	}
}

// Called at the start of the game
void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (PlayerCharacter)
	{
		PlayerMovementComponent = PlayerCharacter->GetCharacterMovement();
	}
}

// Instantly Sets Player's rotation to the controller's rotation based on circumstances
void UPlayerCharacterAnimInstance::UseControllerYawOrNah(bool IsAccelerating, float PlayerSpeed)
{
	if (IsAccelerating && PlayerSpeed > 299.f) // When player is finished turning around, bUseControllerYaw is set to true
	{
		PlayerCharacter->bUseControllerRotationYaw = true;
	}
	else if (!IsAccelerating) // When player stops moving, bUseControllerYaw is set to false
	{
		PlayerCharacter->bUseControllerRotationYaw = false;
	}
}

/**
*	TODO: Instead of using SetActorRotToControlRot, experiment with creating a new walk start animation that plays if the player is facing the opposite direction they want to walk in.
*	There are some bugs with the current implementation of SetActorRotToControlRot, so it would be a good idea to try a better solution later in development.
*/

/** 
*	Sets Player's rotation to the controller's rotation at an interped speed based on circumstances.
*	Used for smoothly turning the player around if the player wants to go in the opposite direction they are currently facing
*/
void UPlayerCharacterAnimInstance::SetActorRotToControlRot(float PlayerSpeed, float DeltaTime, bool IsAccelerating)
{
	// Interps current actor rotation to control rotation based on CharacterTurnSpeed
	FRotator InterpedRotation = FMath::RInterpTo(
		PlayerCharacter->GetActorRotation(),
		PlayerCharacter->GetControlRotation(),
		DeltaTime,
		PlayerCharacter->GetCharacterTurnSpeed()
	);

	UseControllerYawOrNah(IsAccelerating, PlayerSpeed);

	// If player starts moving, interpolate current actor rotation to control rotation. bUseControllerRotationYaw must be false
	if (PlayerSpeed > 10.f && PlayerSpeed < 299.f && !(PlayerCharacter->bUseControllerRotationYaw))
	{
		PlayerCharacter->SetActorRotation(FRotator(0, InterpedRotation.Yaw, 0));
	}
}

	/*FString SpeedMessage = FString::Printf(TEXT("Speed is: %f"), GroundSpeed);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::White, SpeedMessage);
	}*/