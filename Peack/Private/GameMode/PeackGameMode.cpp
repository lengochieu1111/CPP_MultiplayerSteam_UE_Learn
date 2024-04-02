
#include "GameMode/PeackGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"

#include "Controller/PeackPlayerController.h"
#include "PlayerState/PeackPlayerState.h"

namespace MatchState
{
	const FName ShowResult = FName(TEXT("ShowResult"));
	
}

void APeackGameMode::BeginPlay()
{
	Super::BeginPlay();

	this->StartLevelTime = GetWorldTime();
}

// Sever
void APeackGameMode::OnMatchStateSet()
{
	Super::OnMatchStateSet();

	for (FConstPlayerControllerIterator PCI = GetWorld()->GetPlayerControllerIterator(); PCI; ++PCI)
	{
		if (APeackPlayerController* PPC = Cast<APeackPlayerController>(*PCI))
		{
			PPC->GameModeChangedMatchState(GetMatchState());
		}
	}

}

void APeackGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetMatchState() == MatchState::WaitingToStart)
	{
		double TimeLeft = this->TotalTime_Warmup - GetWorldTime();
		TimeLeft += this->StartLevelTime;

		if (TimeLeft <= 0.0)
		{
			StartMatch();
		}
	}
	else if (GetMatchState() == MatchState::InProgress)
	{
		double TimeLeft = this->TotalTime_Match - GetWorldTime();
		TimeLeft += this->StartLevelTime;
		TimeLeft += this->TotalTime_Warmup;

		if (TimeLeft <= 0.0)
		{
			SetMatchState(MatchState::ShowResult);
		}
	}

}

void APeackGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (APeackPlayerController* PPC = Cast<APeackPlayerController>(NewPlayer))
	{
		PPC->GameModeSendInformations(this->StartLevelTime, GetMatchState(), this->TotalTime_Warmup, this->TotalTime_Match);
	}
}

APeackGameMode::APeackGameMode()
{
	bDelayedStart = true;
}

void APeackGameMode::RequestRespawn(ACharacter* GivenCharacter, AController* GivenController)
{
	if (GivenCharacter)
	{
		GivenCharacter->DetachFromControllerPendingDestroy();
	}

	TArray<AActor*> PlayerStartActors;
	UGameplayStatics::GetAllActorsOfClass(
		this, 
		APlayerStart::StaticClass(), 
		PlayerStartActors
	);

	if (PlayerStartActors.IsValidIndex(0))
	{
		RestartPlayerAtPlayerStart(GivenController, PlayerStartActors[0]);
	}

}

void APeackGameMode::HandleCharacterDead(AController* InstigatorController, AController* VictimController)
{
	if (APeackPlayerState* PeackPlayerState = InstigatorController->GetPlayerState<APeackPlayerState>())
	{
		PeackPlayerState->AddOne_Score();
	}

	if (APeackPlayerState* PeackPlayerState = VictimController->GetPlayerState<APeackPlayerState>())
	{
		PeackPlayerState->AddOne_Death();
	}
}

double APeackGameMode::GetWorldTime() const
{
	UWorld* World = GetWorld();

	if (World)
	{
		return World->GetTimeSeconds();
	}

	return 0.0;
}
