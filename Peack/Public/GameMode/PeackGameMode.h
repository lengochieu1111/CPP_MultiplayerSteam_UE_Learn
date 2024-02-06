
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PeackGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PEACK_API APeackGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	APeackGameMode();

	void RequestRespawn(ACharacter* GivenCharacter, AController* GivenController);

	void HandleCharacterDead(AController* InstigatorController, AController* VictimController);

};
