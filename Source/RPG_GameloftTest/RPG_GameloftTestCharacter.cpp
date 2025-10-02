// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPG_GameloftTestCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ARPG_GameloftTestCharacter

ARPG_GameloftTestCharacter::ARPG_GameloftTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Init Health
	MaxHealth = 300.f;
	Health = MaxHealth;

	//ToggleRun
	bIsRunning = true;

	//Skill

	if (Skills.Num() > 0)
	{
		CurrentSkill = Skills[0]; 
	}


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ARPG_GameloftTestCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ARPG_GameloftTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPG_GameloftTestCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPG_GameloftTestCharacter::Look);

		//ToggleRun
		PlayerInputComponent->BindAction("ToggleRun", IE_Pressed, this, &ARPG_GameloftTestCharacter::ToggleRun);


		PlayerInputComponent->BindAction("Skill1", IE_Pressed, this, &ARPG_GameloftTestCharacter::UseSkill1);
		PlayerInputComponent->BindAction("Skill2", IE_Pressed, this, &ARPG_GameloftTestCharacter::UseSkill2);

		// Switch Weapon 1/2
		PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &ARPG_GameloftTestCharacter::SwitchToWeapon1);
		PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ARPG_GameloftTestCharacter::SwitchToWeapon2);


	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARPG_GameloftTestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ARPG_GameloftTestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARPG_GameloftTestCharacter::ToggleRun()
{
	bIsRunning = !bIsRunning;

	if (bIsRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.f; // Run speed
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 230.f; // Walk speed
	}
}

void ARPG_GameloftTestCharacter::UseSkill1()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->UseSkill(0);
	}
}

void ARPG_GameloftTestCharacter::UseSkill2()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->UseSkill(1);
	}
}

void ARPG_GameloftTestCharacter::SwitchToWeapon1()
{
	SwitchWeapon(0);
}

void ARPG_GameloftTestCharacter::SwitchToWeapon2()
{
	SwitchWeapon(1);
}

void ARPG_GameloftTestCharacter::SwitchWeapon(int32 Index)
{
	if (!AvailableWeapons.IsValidIndex(Index)) return;

	// remove old weapon
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	// Spawn new weapon
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(AvailableWeapons[Index], SpawnParams);

	if (NewWeapon)
	{
		NewWeapon->Equip(this);
		CurrentWeapon = NewWeapon;
	}
	// Play anim equip
	if (EquipMontage)
	{
		if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
		{
			Anim->Montage_Play(EquipMontage, 1.f);
		}
	}
}

float ARPG_GameloftTestCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float AppliedDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (AppliedDamage <= 0.f || bIsDead) return 0.f;

	Health -= AppliedDamage;

	if (Health <= 0.f)
	{
		bIsDead = true;
		PlayDeathAnim();
		OnPlayerDied();
	}
	else
	{
		if (HitReactMontage)
		{
			if (UAnimInstance* Anim = GetMesh()->GetAnimInstance())
			{
				if (!Anim->Montage_IsPlaying(HitReactMontage))
				{
					Anim->Montage_Play(HitReactMontage, 1.f);
				}
			}
		}
	}

	return AppliedDamage;
}

void ARPG_GameloftTestCharacter::PlayDeathAnim()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp || !DeathAnim) { EnterRagdoll(); return; }

	// Disable input & movement
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Play anim
	MeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	MeshComp->PlayAnimation(DeathAnim, false);

	// Schedule ragdoll after anim
	DeathTimerHandle.Invalidate();
	float Duration = DeathAnim->GetPlayLength();
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &ARPG_GameloftTestCharacter::EnterRagdoll, Duration, false);
}

void ARPG_GameloftTestCharacter::EnterRagdoll()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp || MeshComp->IsSimulatingPhysics()) return; // check ragdoll 2 time

	if (UCapsuleComponent* Cap = GetCapsuleComponent())
	{
		Cap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	MeshComp->SetAnimInstanceClass(nullptr);
	MeshComp->SetAnimation(nullptr);
	MeshComp->SetAnimationMode(EAnimationMode::AnimationSingleNode);

	MeshComp->SetCollisionProfileName(TEXT("Ragdoll"));
	MeshComp->SetAllBodiesSimulatePhysics(true);
	MeshComp->WakeAllRigidBodies();
	MeshComp->bBlendPhysics = true;

}
