
#include "PlayerState/PeackPlayerState.h"
#include "Controller/PeackPlayerController.h"
#include "Net/UnrealNetwork.h"

/*
* PUBLIC
*/

APeackPlayerState::APeackPlayerState()
{
	NetUpdateFrequency = 1.0f;
	MinNetUpdateFrequency = 1.0f;
}

void APeackPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APeackPlayerState, bReady, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(APeackPlayerState, Dealth, COND_OwnerOnly);

}

void APeackPlayerState::OnRep_Score()
{
	if (IsLocallyControlled())
	{
		UpdateText_Score();
	}
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

void APeackPlayerState::AddOne_Death()
{
	SetDeath(GetDeath() + 1.0f);

	if (IsLocallyControlled())
	{
		UpdateText_Death();
	}
}

void APeackPlayerState::UpdateText_Death()
{
	if (this->PeackPlayerController == nullptr)
	{
		this->PeackPlayerController = Cast<APeackPlayerController>(GetOwningController());
	}

	if (this->PeackPlayerController)
	{
		this->PeackPlayerController->UpdateText_Death(GetDeath());
	}
}

// Sever
void APeackPlayerState::UpdateMVP(const bool bValue)
{
	this->Client_UpdateMVP(bValue);
}

// Owning controller 
void APeackPlayerState::Client_UpdateMVP_Implementation(const bool bValue) //  Implementation
{
	if (this->PeackPlayerController == nullptr)
	{
		this->PeackPlayerController = Cast<APeackPlayerController>(GetOwningController());
	}

	if (this->PeackPlayerController)
	{
		this->PeackPlayerController->ToggleText_MVP(bValue);
	}
}

void APeackPlayerState::SetDeath(const float NewDeath)
{
	this->Dealth = NewDeath;
}

/*
* PROTECTED
*/

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

/*
* PRIVATE
*/

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

void APeackPlayerState::OnRep_Death()
{
	if (IsLocallyControlled())
	{
		UpdateText_Death();
	}
}
