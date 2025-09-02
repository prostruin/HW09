// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ChatPlayerController.h"

#include "EngineUtils.h"
#include "Chat_001/Chat_001.h"
#include "GameMode/ChatGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ChatWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Player/ChatPlayerState.h"


AChatPlayerController::AChatPlayerController()
{
	bReplicates = true;
	bIsMyTurn = false;
}

void AChatPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
	DOREPLIFETIME(ThisClass, TimerText);
	DOREPLIFETIME(ThisClass, bIsMyTurn);
}

void AChatPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}
	
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UChatWidget>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
	
}

void AChatPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		AChatPlayerState* PS = GetPlayerState<AChatPlayerState>();
		if (IsValid(PS) == true)
		{
			FString CombinedMessageString = InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}	
	}
}

void AChatPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	ChatXFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void AChatPlayerController::OnRep_IsMyTurn()
{
	
}

void AChatPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AChatPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AChatGameModeBase* CXGM = Cast<AChatGameModeBase>(GM);
		if (IsValid(CXGM) == true)
		{
			CXGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}
