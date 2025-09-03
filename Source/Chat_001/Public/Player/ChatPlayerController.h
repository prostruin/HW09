// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChatPlayerController.generated.h"

class UChatWidget;
/**
 * 
 */
UCLASS()
class CHAT_001_API AChatPlayerController : public APlayerController
{
	GENERATED_BODY()


public:
	AChatPlayerController();

	UPROPERTY(ReplicatedUsing=OnRep_IsMyTurn, BlueprintReadOnly)
	bool bIsMyTurn;

	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText TimerText;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FText ResultText;

	
	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UChatWidget> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UChatWidget> ChatInputWidgetInstance;	

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

	UFUNCTION()
	void OnRep_IsMyTurn();
	
	
};
