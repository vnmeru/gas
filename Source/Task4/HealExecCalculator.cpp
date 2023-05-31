// Fill out your copyright notice in the Description page of Project Settings.


#include "HealExecCalculator.h"

#include "PlayerBaseAttributeSet.h"

struct GHealCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Heal)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)

	GHealCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerBaseAttributeSet, Heal, Source, true)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerBaseAttributeSet, Health, Target, true)
	}
};
static const GHealCapture& HealCapture()
{
	static GHealCapture HCapture;
	return HCapture;
}
void UHealExecCalculator::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if (!TargetASC)
	{
		return;
	}

	const FGameplayEffectSpec& OwningSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = OwningSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = OwningSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float AppliedHeal = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealCapture().HealDef, EvaluationParameters, AppliedHeal);
	AppliedHeal += OwningSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Base.Heal")));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(HealCapture().HealthProperty, EGameplayModOp::Additive, AppliedHeal));
}
