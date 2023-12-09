
#include "PlayerState/PeackPlayerState.h"
#include "Controller/PeackPlayerController.h"
#include "Net/UnrealNetwork.h"

void APeackPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APeackPlayerState, bReady, COND_OwnerOnly);

}

void APeackPlayerState::AddOne_Score()
{
	SetScore(GetScore() + 1.0f);

	if (IsLocallyControlled())
	{
		UpdateText_Score();
	}
}

void APeackPlayerState::UpdateText_Score()
{
	if (this->PeackPlayerController == nullptr)
	{
		this->PeackPlayerController = Cast<APeackPlayerController>(GetOwningController());
	}

	if (this->PeackPlayerController)
	{
		this->PeackPlayerController->UpdateText_Score(GetScore());
	}
}

void APeackPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		this->bReady = true;

		if (IsLocallyControlled())
		{
			OnRep_Ready();
		}
	}
}

bool APeackPlayerState::IsLocallyControlled() const
{
	AController* MyController = GetOwningController();
	return (MyController && MyController->IsLocalController());
}

void APeackPlayerState::OnRep_Ready()
{
	if (this->PeackPlayerController = Cast<APeackPlayerController>(GetOwningController()))
	{
		this->PeackPlayerController->ReadyPlayerState(this);
	}
}
