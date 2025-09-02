// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ChatGameModeBase.h"

#include "EngineUtils.h"
#include "GameState/ChatGameStateBase.h"
#include "Misc/LowLevelTestAdapter.h"
#include "Player/ChatPlayerController.h"
#include "Player/ChatPlayerState.h"


void AChatGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	BaseTime = 30;

	AChatPlayerController* CXPlayerController = Cast<AChatPlayerController>(NewPlayer);
	if (IsValid(CXPlayerController) == true)
	{
		CXPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		
		
		AllPlayerControllers.Add(CXPlayerController);

		AChatPlayerState* CXPS = CXPlayerController->GetPlayerState<AChatPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		AChatGameStateBase* CXGameStateBase =  GetGameState<AChatGameStateBase>();
		if (IsValid(CXGameStateBase) == true)
		{
			CXGameStateBase->MulticastRPCBroadcastLoginMessage(CXPS->PlayerNameString);
		}

		if (AllPlayerControllers.Num() >= 2) // 2명이상 접속시 게임 시작.
		{
			// 게임 시작 시 컨트롤러에 UI noti 업데이트
			for (AChatPlayerController* PlayerController : AllPlayerControllers)
			{
				PlayerController->NotificationText = FText::FromString(TEXT("게임이 시작됩니다!"));
			}
			BeginTurn();
		}
	}
}

FString AChatGameModeBase::GenerateSecretNumber()
{

	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });
	
	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool AChatGameModeBase::IsGuessNumberString(const FString& InNumberString)
{

	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			
			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
		
	} while (false);	

	return bCanPlay;
}

FString AChatGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else 
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;				
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AChatGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();

	CurrentPlayerIndex = -1;
}

void AChatGameModeBase::PrintChatMessageString(AChatPlayerController* InChattingPlayerController,
	const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		AChatPlayerController* CurrentTurnPlayer = AllPlayerControllers[CurrentPlayerIndex];
		if (InChattingPlayerController != CurrentTurnPlayer)
		{
			// 턴이 아닌 플레이어면 메세지 출력 x 
			InChattingPlayerController->NotificationText = FText::FromString(TEXT("당신의 턴이 아닙니다"));
			return;
		}

		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		IncreaseGuessCount(InChattingPlayerController);

		
		for (TActorIterator<AChatPlayerController> It(GetWorld()); It; ++It)
		{
			AChatPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController) == true)
			{
				AChatPlayerState* PS = InChattingPlayerController->GetPlayerState<AChatPlayerState>();
				if (IsValid(PS) == true)
				{
					FString PlayerInfo = TEXT("(") + FString::FromInt(PS->CurrentGuessCount) + TEXT("/") + FString::FromInt(PS->MaxGuessCount) + TEXT(")");
				

				
					FString CombinedMessageString = InChatMessageString + PlayerInfo + TEXT(" -> ") + JudgeResultString;
					CXPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

					int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
					JudgeGame(InChattingPlayerController, StrikeCount);
				}
			}
		}
		OnTurnFinished();
	}
	else
	{
		for (TActorIterator<AChatPlayerController> It(GetWorld()); It; ++It)
		{
			AChatPlayerController* CXPlayerController = *It;
			if (IsValid(CXPlayerController) == true)
			{
				CXPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void AChatGameModeBase::IncreaseGuessCount(AChatPlayerController* InChattingPlayerController)
{
	AChatPlayerState* CXPS = InChattingPlayerController->GetPlayerState<AChatPlayerState>();
	if (IsValid(CXPS) == true)
	{
		CXPS->CurrentGuessCount++;
	}
}

void AChatGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& CXPlayerController : AllPlayerControllers)
	{
		AChatPlayerState* CXPS = CXPlayerController->GetPlayerState<AChatPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->CurrentGuessCount = 0;
		}
	}
}

void AChatGameModeBase::JudgeGame(AChatPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		AChatPlayerState* CXPS = InChattingPlayerController->GetPlayerState<AChatPlayerState>();
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			if (IsValid(CXPS) == true)
			{
				FString CombinedMessageString = CXPS->PlayerNameString + TEXT(" has won the game.");
				CXPlayerController->NotificationText = FText::FromString(CombinedMessageString);
				CXPlayerController->TimerText = FText(); // 타이머 초기화

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CXPlayerController : AllPlayerControllers)
		{
			AChatPlayerState* CXPS = CXPlayerController->GetPlayerState<AChatPlayerState>();
			if (IsValid(CXPS) == true)
			{
				if (CXPS->CurrentGuessCount < CXPS->MaxGuessCount)
				{
					bIsDraw = false;
					CXPlayerController->TimerText = FText(); // 타이머 초기화
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& CXPlayerController : AllPlayerControllers)
			{
				CXPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
				CXPlayerController->TimerText = FText(); // 타이머 초기화

				ResetGame();
			}
		}
	}
}

void AChatGameModeBase::BeginTurn()
{
	// 이전 플레이어의 턴 알림 및 타이머 초기화
	if (CurrentPlayerIndex != -1)
	{
		AChatPlayerController* PreviousPlayer = AllPlayerControllers[CurrentPlayerIndex];
		if (IsValid(PreviousPlayer))
		{
			PreviousPlayer->NotificationText = FText::FromString(TEXT("상대방의 턴입니다."));
			PreviousPlayer->TimerText = FText();
		}
	}
    
	// 다음 플레이어의 인덱스 계산
	CurrentPlayerIndex = (CurrentPlayerIndex + 1) % AllPlayerControllers.Num();
    
	// 새 턴의 플레이어에게 알림 및 타이머 시작
	AChatPlayerController* CurrentPlayer = AllPlayerControllers[CurrentPlayerIndex];
	if (IsValid(CurrentPlayer))
	{
		CurrentPlayer->NotificationText = FText::FromString(TEXT("당신의 턴입니다!"));
        
		// 타이머 시작
		TurnTimeRemaining = BaseTime;
		GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &AChatGameModeBase::HandleTurnTick, 1.0f, true);
        
		// 초기 타이머 값 설정
		CurrentPlayer->TimerText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(TurnTimeRemaining));
	}
}

void AChatGameModeBase::HandleTurnTick()
{
	TurnTimeRemaining--;

	// 현재 턴 플레이어의 위젯만 업데이트
	AChatPlayerController* CurrentPlayer = AllPlayerControllers[CurrentPlayerIndex];
	if (IsValid(CurrentPlayer))
	{
		CurrentPlayer->TimerText = FText::Format(FText::FromString(TEXT("{0}초")), FText::AsNumber(TurnTimeRemaining));
	}

	if (IsValid(CurrentPlayer))
	{
		if (TurnTimeRemaining <= BaseTime/2)
		{
			CurrentPlayer->NotificationText = FText::FromString(TEXT("빨리 숫자를 입력하세요! 시간이 지나면 기회가 소모됩니다."));
		}
	}
    
	// 시간이 0이 되면 턴 종료
	if (TurnTimeRemaining <= 0)
	{
		IncreaseGuessCount(CurrentPlayer);
		OnTurnFinished();
	}
}

void AChatGameModeBase::OnTurnFinished()
{
	GetWorldTimerManager().ClearTimer(TurnTimerHandle);
    
	AChatPlayerController* CurrentPlayer = AllPlayerControllers[CurrentPlayerIndex];
	if (IsValid(CurrentPlayer))
	{
		CurrentPlayer->NotificationText = FText::FromString(TEXT("시간 초과!"));
		CurrentPlayer->TimerText = FText();
	}
    
	// 다음 턴 시작
	BeginTurn();
}


