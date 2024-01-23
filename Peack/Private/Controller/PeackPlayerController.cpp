

#include "Controller/PeackPlayerController.h"

#include "Widget/CharacterWidget.h"
#include "Widget/PlayerStateWidget.h"
#include "PlayerState/PeackPlayerState.h"


void APeackPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocalController())
	{
		double TimeLeft = this->TotalTime_Match - GetWorldTime_Server();
		int CurrentCountdown = FMath::CeilToInt(TimeLeft);

		if (CurrentCountdown != this->LastCountdown)
		{
			this->UpdateText_Countdown(CurrentCountdown);
			this->LastCountdown = CurrentCountdown;
		}
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
	if (GivenPlayerState)
	{
		CreateWidget_PlayerState();
		UpdateText_Score(GivenPlayerState->GetScore());
		UpdateText_Death(GivenPlayerState->GetDeath());
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

void APeackPlayerController::UpdateText_Countdown(int TimeLeft)
{
	if (this->Widget_PlayerState)
	{
		this->Widget_PlayerState->UpdateText_Countdown(TimeLeft);
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

