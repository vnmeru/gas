// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "AbilitySystemComponent.h"
#include "MyCharacter.h"
#include "MyPlayerState.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalPlayerController())
	{
		//Set PlayerName
		const UMySaveGame* LoadedGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Leaderboard"), 0));

		if (LoadedGame)
		{
			SetPlayerName(LoadedGame->NickName.ToString());
		}
		else
		{
			SetPlayerName("Player");
		}
	}
}

void AMyPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	//auto Character* CharacterBase = Cast<AMyCharacterBase>(P);
	//if (CharacterBase)
	//{
	//	CharacterBase->GetAbilitySystemComponent()->InitAbilityActorInfo(CharacterBase, CharacterBase);
	//}
}

void AMyPlayerController::SetPlayerName_Implementation(const FString& Name)
{
	AMyPlayerState* GamePlayerState = Cast<AMyPlayerState>(PlayerState);

	if (GamePlayerState)
	{
		GamePlayerState->SetPlayerName(Name.IsEmpty() ? TEXT("Player") : Name);
	}
}
