// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HealthComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "GameplayEffectTypes.h"
#include "MyCharacterBase.generated.h"

class UAbilitySystemComponent;
class UPlayerBaseAttributeSet;

DECLARE_LOG_CATEGORY_CLASS(LogMyCharacter, Log, All);

UCLASS()
class TASK4_API AMyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AMyCharacterBase();
	void PossessedBy(AController* NewController) override;
	//Called on client when he recieves the playerstate
	void OnRep_PlayerState() override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void ApplyAbilities();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void GrantSprintAbility();
	//UFUNCTION()
	//void RemoveSprintAbility();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Axis);

	void MoveRight(float Axis);

	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);
public:

	UPROPERTY(VisibleAnywhere)
	float WalkSpeed = 600;

	UPROPERTY(VisibleAnywhere)
	float LookUpRate = 50.f;

	UPROPERTY(VisibleAnywhere)
	float TurnRate = 50.f;

	UPROPERTY(EditAnywhere)
	float SprintSpeed = 400;

	UFUNCTION(NetMulticast, Reliable)
	void StartMovement();

	UFUNCTION(NetMulticast, Reliable)
	void SetLocalName();

	UFUNCTION(NetMulticast, Reliable)
	void StartSpectating();

	UFUNCTION(Server, Reliable)
	void Sprint();

	UFUNCTION(Server, Reliable)
	void ForceDeath();

	UFUNCTION(Server, Reliable)
	void StopSprinting();

	UFUNCTION(Client, Reliable)
	void Die();

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* NameTagWidget;

	UPROPERTY(VisibleAnywhere)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
	TArray<TSubclassOf<UGameplayAbilityEx>> DefaultAbilities;

	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite, Category = "Ability")
	UDataTable* DefaultAttributeValues;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
