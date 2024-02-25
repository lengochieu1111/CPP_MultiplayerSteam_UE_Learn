
#include "GameMode/PeackGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"

#include "Controller/PeackPlayerController.h"
#include "PlayerState/PeackPlayerState.h"

void APeackGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (APeackPlayerController* PPC = Cast<APeackPlayerController>(NewPlayer))
	{
		PPC->GameModeSendInformations(GetMatchState());
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
