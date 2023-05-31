// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterBase.h"

#include "MyPlayerController.h"
#include "MyPlayerState.h"
#include "NameTagWidget.h"
#include "Task4GameStateMP.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "GA_Sprint.h"
#include "PlayerBaseAttributeSet.h"

// Sets default values
AMyCharacterBase::AMyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//Health = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	NameTagWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameTag"));
	NameTagWidget->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	//AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilityComponent"));
	
	bReplicates = true;
}

//Called by server
void AMyCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		AbilitySystemComponent = Cast<UAbilitySystemComponent>(PS->GetAbilitySystemComponent());
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
		ApplyAbilities();
	}
}

void AMyCharacterBase::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	const float CurrentHealth = Data.NewValue;
	const UWorld* World = GetWorld();
	if (World)
	{
		AMyPlayerController* MyController = Cast<AMyPlayerController>(GEngine->GetFirstLocalPlayerController(World));
		if (MyController)
		{
			MyController->OnHealthUpdatedEvent.Broadcast(CurrentHealth);
		}

		if (CurrentHealth <= 0.f || FMath::IsNearlyZero(CurrentHealth))
		{
			if (MyController)
			{
				StartSpectating();
			}

			Die();
		}
	}
}

void AMyCharacterBase::ApplyAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		AbilitySystemComponent->InitStats(UPlayerBaseAttributeSet::StaticClass(), DefaultAttributeValues);

		for (auto& DefaultAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(DefaultAbility, 1, static_cast<int32>(DefaultAbility.GetDefaultObject()->AbilityInputID), this));
		}
	}

}

// Called when the game starts or when spawned
void AMyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetCharacterMovement()->AirControl = 1.0f;
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	if (MyController)
	{
		MyController->bShowMouseCursor = false;
		MyController->SetInputMode(FInputModeGameOnly{});
	}
}

//Called on client only!
void AMyCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		AbilitySystemComponent->AddSet<UPlayerBaseAttributeSet>();
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(PS->GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
		ApplyAbilities();
	}

}

// Called every frame
void AMyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyCharacterBase::GrantSprintAbility()
{
}

// Called to bind functionality to input
void AMyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ThisClass::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ThisClass::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ThisClass::Jump);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Released, this, &ThisClass::StopJumping);

	//
	//PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ThisClass::GrantSprintAbility);
	//PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ThisClass::RemoveSprintAbility);

	PlayerInputComponent->BindAxis("TurnRate", this, &ThisClass::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ThisClass::LookUpAtRate);

	const FGameplayAbilityInputBinds Binds("Confirm", "Cancel", "EGASAbilityInputID", static_cast<int32>(EGASAbilityInputID::Confirm), static_cast<int32>(EGASAbilityInputID::Cancel));
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, Binds);
	}
}

void AMyCharacterBase::MoveForward(float Axis)
{
	if (!FMath::IsNearlyZero(Axis))
	{
		AddMovementInput(GetActorForwardVector(), Axis);
	}
}
void AMyCharacterBase::MoveRight(float Axis)
{
	//SideWaysDir = Axis;

	//if (GetCharacterMovement()->MovementMode == MOVE_Falling)
	//{
	//	//Strafe!
	//	APlayerCameraManager* CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	//	FVector VectorVelocity = GetVelocity();
	//	FVector VecCameraForward = CamManager->GetCameraRotation().Vector();

	//	VectorVelocity.Z = 0.0f;
	//	VecCameraForward.Z = 0.0f;

	//	VectorVelocity.Normalize();
	//	VecCameraForward.Normalize();

	//	float AngleVelocityRad = FMath::RadiansToDegrees(FMath::Atan2(VectorVelocity.Y, VectorVelocity.X));
	//	float AngleCameraForwardRad = FMath::RadiansToDegrees(FMath::Atan2(VecCameraForward.Y, VecCameraForward.X));
	//	float AngleDiff = FMath::ClampAngle(AngleVelocityRad - AngleCameraForwardRad, -179.0f, 179.0f);

	//	// Forward: Left +, Right -, Backwards: Left -, Right +
	//	//FVector VecStrafe = VecCameraForward.RotateAngleAxis(90.0f, FVector(0.0f, 0.0f, 1.0f)) * -SideWaysDir * ForWardDir * AirAccelerateValue;
	//	AddMovementInput(GetActorRightVector(), -SideWaysDir * ForWardDir * AirAccelerateValue, true);

	//	UE_LOG(LogTemp, Display, TEXT("AngleDiff=%f, Axis=%f, Accel=%f"), AngleDiff, Axis, -SideWaysDir * ForwardDir * AirAccelerateValue);

	if (!FMath::IsNearlyZero(Axis))
	{
		AddMovementInput(GetActorRightVector(), Axis);
	}

}
void AMyCharacterBase::TurnAtRate(float Rate)
{
	//if (!FMath::IsNearlyZero(Rate))
	//{
	AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds());
	//}
}

void AMyCharacterBase::LookUpAtRate(float Rate)
{
	//if (!FMath::IsNearlyZero(Rate))
	//{
	AddControllerPitchInput(Rate * LookUpRate * GetWorld()->GetDeltaSeconds());
	//}
}

void AMyCharacterBase::Sprint_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed += SprintSpeed;
	UE_LOG(LogMyCharacter, Display, TEXT("Sprint = %f"), GetCharacterMovement()->MaxWalkSpeed);
}



void AMyCharacterBase::StopSprinting_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed -= SprintSpeed;
	UE_LOG(LogMyCharacter, Display, TEXT("StopSprinting = %f"), GetCharacterMovement()->MaxWalkSpeed);
}


void AMyCharacterBase::Die_Implementation()
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GEngine->GetFirstLocalPlayerController(World));
	if (PlayerController)
	{
		if (PlayerController->bIsDead)
		{
			return;
		}

		PlayerController->bIsDead = true;
	}

	SetActorTickEnabled(false);

	if (World && PlayerController)
	{
		PlayerController->SetPause(true);
		PlayerController->bShowMouseCursor = true;
		ATask4GameStateMP* GameState = Cast<ATask4GameStateMP>(UGameplayStatics::GetGameState(World));
		if (GameState)
		{
			PlayerController->OnCharacterDeath.Broadcast(GameState->GetRoundTime());
		}
	}
}

void AMyCharacterBase::ForceDeath_Implementation()
{
	//Health->IncreaseValue(-999);
}

float AMyCharacterBase::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                   AActor* DamageCauser)
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController());
	if (PlayerController)
	{
		if (PlayerController->bIsDead)
		{
			return Damage;
		}
	}
	

	return Damage;
}

void AMyCharacterBase::SetLocalName_Implementation()
{
	UNameTagWidget* NameTag = Cast<UNameTagWidget>(NameTagWidget->GetWidget());
	if (NameTag)
	{
		APlayerState* MyPlayerState = Cast<APlayerState>(GetPlayerState());
		if (MyPlayerState)
		{
			//NameTag->PlayerName->SetText(FText::FromString(MyPlayerState->GetPlayerName()));
		}
	}
}

void AMyCharacterBase::StartSpectating_Implementation()
{
	APlayerController* MyController = Cast<APlayerController>(GetController());
	if (MyController)
	{
		MyController->ChangeState(NAME_Spectating);
	}
}

void AMyCharacterBase::StartMovement_Implementation()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

