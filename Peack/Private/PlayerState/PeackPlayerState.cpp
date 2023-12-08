
#include "PlayerState/PeackPlayerState.h"
#include "Controller/PeackPlayerController.h"
#include "Net/UnrealNetwork.h"

void APeackPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APeackPlayerState, bReady, COND_OwnerOnly);

}

void APeackPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		this->bReady = true;

		if (IsLocallyControlled())
		{
			if (APeackPlayerController* PeackPlayerController = Cast<APeackPlayerController>(GetOwningController()))
			{
				PeackPlayerController->ReadyPlayerState();
			}
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
	if (APeackPlayerController* PeackPlayerController = Cast<APeackPlayerController>(GetOwningController()))
	{
		PeackPlayerController->ReadyPlayerState();
	}
}
