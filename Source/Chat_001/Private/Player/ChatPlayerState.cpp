// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ChatPlayerState.h"

#include "Net/UnrealNetwork.h"

AChatPlayerState::AChatPlayerState()
: PlayerNameString(TEXT("None"))
, CurrentGuessCount(0)
, MaxGuessCount(3)
{
	bReplicates = true;
}

void AChatPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount); // Q. MaxGuessCount가 프로퍼티 레플리케이션될 필요가 있을까?

	
}

FString AChatPlayerState::GetPlayerInfoString()
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerNameString;
}

int32 AChatPlayerState::GetCurrentcount()
{
	return CurrentGuessCount;
}

int32 AChatPlayerState::GetMaxcount()
{
	return MaxGuessCount;
}

