// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_Sprint.h"

#include "MyCharacterBase.h"

AMyCharacterBase* GetMyCharacter(const FGameplayAbilityActorInfo* ActorInfo)
{
	auto* Actor = ActorInfo->AvatarActor.Get();

	if (!Actor)
	{
		return nullptr;
	}

	auto* Character = Cast<AMyCharacterBase>(Actor);
	if (!Character)
	{
		return nullptr;
	}

	return Character;
}

UGA_Sprint::UGA_Sprint()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Sprint")));
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	AbilityInputID = EGASAbilityInputID::Sprint;
}

void UGA_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                 const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//TODO: Add Authority check
	if (auto* Character = GetMyCharacter(ActorInfo))
	{
		Character->Sprint();
	}
}

void UGA_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                               const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{

	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	if (auto* Character = GetMyCharacter(ActorInfo))
	{
		Character->StopSprinting();
	}
}

void UGA_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	CancelAbility(Handle, ActorInfo, ActivationInfo, true);
}
