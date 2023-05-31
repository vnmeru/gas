// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

#include "AbilitySystemComponent.h"
#include "MyPlayerController.h"
#include "MySaveGame.h"
#include "PlayerBaseAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
{
	bUseCustomPlayerNames = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilityComponent"));
	PlayerBaseAttributeSet = CreateDefaultSubobject<UPlayerBaseAttributeSet>("HealthAttributeSet");
	AbilitySystemComponent->SetIsReplicated(true);
}

void AMyPlayerState::OnRepMovementChanged()
{
	AMyPlayerController* Controller = Cast<AMyPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	Controller->OnGameStartEvent.Broadcast();
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bCanMove);
}
