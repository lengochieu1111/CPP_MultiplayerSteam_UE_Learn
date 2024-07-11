
#include "GameState/PeackGameState.h"

#include "PlayerState/PeackPlayerState.h"

APeackGameState::APeackGameState()
{
	this->NetUpdateFrequency = 10.0f;
	this->MinNetUpdateFrequency = 7.0f;
}

void APeackGameState::UpdateMVP(APeackPlayerState* InstigatorPlayerState)
{
	if (InstigatorPlayerState == nullptr) return;

	if (InstigatorPlayerState == this->CurrentMVP) return;

	if (InstigatorPlayerState->GetScore() > this->CurrentMVP->GetScore())
	{
		this->CurrentMVP = InstigatorPlayerState;
	}
}
