// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PlayerBaseAttributeSet.generated.h"

#define GPA_ACCESSOR(x)\
GAMEPLAYATTRIBUTE_VALUE_GETTER(x);\
GAMEPLAYATTRIBUTE_VALUE_SETTER(x);\
GAMEPLAYATTRIBUTE_VALUE_INITTER(x);\
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ThisClass, x)

UCLASS()
class TASK4_API UPlayerBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes");
	FGameplayAttributeData Health;
	GPA_ACCESSOR(Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes");
	FGameplayAttributeData Damage;
	GPA_ACCESSOR(Damage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes");
	FGameplayAttributeData Heal;
	GPA_ACCESSOR(Heal);
};
