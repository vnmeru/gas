// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerHUDWidget.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdated, float, Number);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStart);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDeath, float, Time);


UCLASS()
class TASK4_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void AcknowledgePossession(APawn* P) override;

	FOnHealthUpdated OnHealthUpdatedEvent;

	FCharacterDeath OnCharacterDeath;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDeathScreenWidget> DeathScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerHUDWidget> PlayerHUDWidget;

	FOnGameStart OnGameStartEvent;

	bool bIsDead = false;
private:

	UFUNCTION(Server, Reliable)
	void SetPlayerName(const FString& Name);
};
