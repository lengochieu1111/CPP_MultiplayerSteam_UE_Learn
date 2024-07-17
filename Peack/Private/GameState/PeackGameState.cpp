
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

	if (this->CurrentMVP == nullptr)
	{
		InstigatorPlayerState->UpdateMVP(true);

		this->CurrentMVP = InstigatorPlayerState;
		return;
	}

	if (InstigatorPlayerState->GetScore() > this->CurrentMVP->GetScore())
	{
		InstigatorPlayerState->UpdateMVP(true);
		this->CurrentMVP->UpdateMVP(false);

		this->CurrentMVP = InstigatorPlayerState;
	}
}

