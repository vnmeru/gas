// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;


UCLASS()
class TASK4_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMyPlayerState();
	UAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	 UPlayerBaseAttributeSet* GetAttributeSet() { return PlayerBaseAttributeSet; }
	UFUNCTION()
	void OnRepMovementChanged();
private:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

public:
	UPROPERTY(VisibleAnywhere)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	UPlayerBaseAttributeSet* PlayerBaseAttributeSet;

	UPROPERTY(ReplicatedUsing=OnRepMovementChanged)
	bool bCanMove;
};
