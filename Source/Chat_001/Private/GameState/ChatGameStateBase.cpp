// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/ChatGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ChatPlayerController.h"

void AChatGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			AChatPlayerController* CXPC = Cast<AChatPlayerController>(PC);
			if (IsValid(CXPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CXPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}

