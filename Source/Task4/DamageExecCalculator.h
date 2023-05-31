// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecCalculator.generated.h"

/**
 * 
 */
UCLASS()
class TASK4_API UDamageExecCalculator : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
