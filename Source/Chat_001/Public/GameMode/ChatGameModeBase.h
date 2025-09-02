// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChatGameModeBase.generated.h"

class AChatPlayerController;
/**
 * 
 */
UCLASS()
class CHAT_001_API AChatGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;
	
	void PrintChatMessageString(AChatPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(AChatPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(AChatPlayerController* InChattingPlayerController, int InStrikeCount);
	
protected:
	FString SecretNumberString;

	TArray<TObjectPtr<AChatPlayerController>> AllPlayerControllers;

	int32 CurrentPlayerIndex;

	FTimerHandle TurnTimerHandle;
	int32 TurnTimeRemaining;

	int32 BaseTime;

	FString Answer;

	void BeginTurn();
	void HandleTurnTick();
	void OnTurnFinished();



	
};
