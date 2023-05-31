// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "HealExecCalculator.generated.h"

/**
 * 
 */
UCLASS()
class TASK4_API UHealExecCalculator : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
