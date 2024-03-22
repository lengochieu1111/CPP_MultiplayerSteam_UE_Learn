

#include "Controller/PeackPlayerController.h"

#include "Widget/CharacterWidget.h"
#include "Widget/PlayerStateWidget.h"
#include "PlayerState/PeackPlayerState.h"
#include "GameFramework/GameMode.h"
#include "Widget/WarmupWidget.h"


void APeackPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocalController())
	{
		UpdateCountdown();
	}

}

void APeackPlayerController::UpdateCountdown()
{
	if (this->CurrentMatchState == MatchState::WaitingToStart)
	{
		UpdateCountdown_Warmup();
	}
	else if (this->CurrentMatchState == MatchState::InProgress)
	{
		UpdateCountdown_InMatch();
	}
}

void APeackPlayerController::UpdateCountdown_Warmup()
{
	double TimeLeft = this->TotalTime_Warmup - GetWorldTime_Server();
	TimeLeft += this->StartLevelTime;
	int CurrentCountdown = FMath::CeilToInt(TimeLeft);

	if (CurrentCountdown != this->LastCountdown)
	{
		// Warmup
		// PlayerState
		if (this->Widget_Warmup)
		{
			this->Widget_Warmup->UpdateText_Countdown(CurrentCountdown);
		}
		this->LastCountdown = CurrentCountdown;
	}
}

void APeackPlayerController::UpdateCountdown_InMatch()
{
	double TimeLeft = this->TotalTime_Match - GetWorldTime_Server();
	TimeLeft += this->StartLevelTime;
	TimeLeft += this->TotalTime_Warmup;

	int CurrentCountdown = FMath::CeilToInt(TimeLeft);

	if (CurrentCountdown != this->LastCountdown)
	{
		if (this->Widget_PlayerState)
		{
			this->Widget_PlayerState->UpdateText_Countdown(CurrentCountdown);
		}
		this->LastCountdown = CurrentCountdown;
	}
}

void APeackPlayerController::ReceivedPlayer()
{
	if (!HasAuthority() && IsLocalController())
	{
		Server_RequestServerTime(GetWorldTime());
	}
}

void APeackPlayerController::Server_RequestServerTime_Implementation(double RequestTimeFromClient) // Implementation
{
	Client_ReportServerTimeToClient(RequestTimeFromClient, GetWorldTime());
}

void APeackPlayerController::Client_ReportServerTimeToClient_Implementation(double RequestTimeFromClient, double ReceverTimeFromServer) // Implementation
{
	double RoundTripTime = GetWorldTime() - RequestTimeFromClient;
	double CurrentSeverTime = ReceverTimeFromServer + (0.5 * RoundTripTime);
	this->Detal_Server_Client = CurrentSeverTime - GetWorldTime();
}

void APeackPlayerController::CreateWidget_Character()
{
	if (this->Widget_Character) return;

	this->Widget_Character = CreateWidget< UCharacterWidget>(this, this->WidgetClass_Character);

	if (this->Widget_Character)
	{
		this->Widget_Character->AddToViewport();
	}

}

void APeackPlayerController::ReadyPlayerState(APeackPlayerState* GivenPlayerState)
{
	this->PeackPlayerState = GivenPlayerState;

	if (this->CurrentMatchState == MatchState::InProgress)
	{
		if (this->PeackPlayerState)
		{
			this->CreateWidget_PlayerState();
			this->UpdateText_Score(this->PeackPlayerState->GetScore());
			this->UpdateText_Death(this->PeackPlayerState->GetDeath());
		}
	}
}

void APeackPlayerController::CreateWidget_PlayerState()
{
	if (this->Widget_PlayerState) return;

	this->Widget_PlayerState = CreateWidget< UPlayerStateWidget>(this, this->WidgetClass_PlayerState);

	if (this->Widget_PlayerState)
	{
		this->Widget_PlayerState->AddToViewport();
	}
}

void APeackPlayerController::CreateWidget_Warmup()
{
	if (this->Widget_Warmup) return;

	this->Widget_Warmup = CreateWidget< UWarmupWidget>(this, this->WidgetClass_Warmup);

	if (this->Widget_Warmup)
	{
		this->Widget_Warmup->AddToViewport();
	}
}

void APeackPlayerController::UpdateBar_Health(float Health, float MaxHealth)
{
	if (this->Widget_Character)
	{
		this->Widget_Character->UpdateBar_Health(Health, MaxHealth);
	}
}

void APeackPlayerController::UpdateText_Score(float GivenScore)
{
	if (this->Widget_PlayerState)
	{
		this->Widget_PlayerState->UpdateText_Score(GivenScore);
	}
}

void APeackPlayerController::UpdateText_Death(float GivenDeath)
{
	if (this->Widget_PlayerState)
	{
		this->Widget_PlayerState->UpdateText_Death(GivenDeath);
	}
}

// Sever
void APeackPlayerController::GameModeChangedMatchState(const FName NewMatchState)
{
	this->Client_GameModeChangedMatchState(NewMatchState);
}

// PC: Owning this player controller (Sever, Client)
void APeackPlayerController::Client_GameModeChangedMatchState_Implementation(const FName NewMatchState) // Implementation
{
	HandleMatchState(NewMatchState);
}

// Sever
void APeackPlayerController::GameModeSendInformations(const double SLT, const FName GivenMatchState,const double TotalTimeWarmup,const double TotalTimeMatch)
{
	this->TotalTime_Warmup = TotalTimeWarmup;
	this->TotalTime_Match = TotalTimeMatch;
	this->StartLevelTime = SLT;

	Client_GameModeSendInformations(SLT, GivenMatchState, TotalTimeWarmup, TotalTimeMatch);
}

// PC: Owning this player controller (Sever, Client) 
void APeackPlayerController::Client_GameModeSendInformations_Implementation(const double SLT, const FName GivenMatchState, const double TotalTimeWarmup, const double TotalTimeMatch) // Implementation
{
	this->TotalTime_Warmup = TotalTimeWarmup;
	this->TotalTime_Match = TotalTimeMatch;
	this->StartLevelTime = SLT;

	HandleMatchState(GivenMatchState);
}

void APeackPlayerController::HandleMatchState(const FName GivenMatchState)
{
	if (this->CurrentMatchState == GivenMatchState)
	{
		return;
	}

	this->CurrentMatchState = GivenMatchState;

	if (GivenMatchState == MatchState::WaitingToStart)
	{
		this->CreateWidget_Warmup();
	}
	else if (GivenMatchState == MatchState::InProgress)
	{
		if (this->Widget_Warmup)
		{
			this->Widget_Warmup->RemoveFromParent();
		}

		if (this->PeackPlayerState)
		{
			this->CreateWidget_PlayerState();
			this->UpdateText_Score(this->PeackPlayerState->GetScore());
			this->UpdateText_Death(this->PeackPlayerState->GetDeath());
		}

	}
}

double APeackPlayerController::GetWorldTime() const
{
	UWorld* World = GetWorld();
	if (World)
		return World->GetTimeSeconds();

	return 0.0;
}

double APeackPlayerController::GetWorldTime_Server() const
{
	return GetWorldTime() + this->Detal_Server_Client;
}



