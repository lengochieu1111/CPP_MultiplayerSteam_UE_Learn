
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PeackGameState.generated.h"

class APeackPlayerState;

/**
 * 
 */
UCLASS()
class PEACK_API APeackGameState : public AGameState
{
	GENERATED_BODY()

	// FUNCTION
public:
	APeackGameState();

	void UpdateMVP(APeackPlayerState* InstigatorPlayerState);

	// PROPERTY
public:
	UPROPERTY()
	TObjectPtr<APeackPlayerState> CurrentMVP;
	
};
