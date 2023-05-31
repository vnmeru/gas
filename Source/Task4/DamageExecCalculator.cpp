// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecCalculator.h"

#include "AbilitySystemComponent.h"
#include "PlayerBaseAttributeSet.h"


struct GDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health)


	GDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerBaseAttributeSet, Damage, Source, true)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPlayerBaseAttributeSet, Health, Target, true)
	}
};
static const GDamageCapture& DamageCapture()
{
	static GDamageCapture DStatics;
	return DStatics;
}
void UDamageExecCalculator::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                   FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Handle everything by ourselfs
	//Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	if(!TargetASC)
	{
		return;
	}
	const FGameplayEffectSpec& OwningSpec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = OwningSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = OwningSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float AppliedDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageCapture().DamageDef, EvaluationParameters, AppliedDamage);
	AppliedDamage += OwningSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Base.Damage")));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageCapture().HealthProperty, EGameplayModOp::Additive, AppliedDamage));
}
