// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Task4.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityEx.generated.h"

/**
 * 
 */
UCLASS()
class TASK4_API UGameplayAbilityEx : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "AbilityInput")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
};
