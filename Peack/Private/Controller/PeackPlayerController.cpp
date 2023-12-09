

#include "Controller/PeackPlayerController.h"

#include "Widget/CharacterWidget.h"
#include "Widget/PlayerStateWidget.h"
#include "PlayerState/PeackPlayerState.h"


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
