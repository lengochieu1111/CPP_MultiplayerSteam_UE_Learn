
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

	virtual void PostLogin(APlayerController* NewPlayer) override;

	APeackGameMode();

	void RequestRespawn(ACharacter* GivenCharacter, AController* GivenController);

	void HandleCharacterDead(AController* InstigatorController, AController* VictimController);

protected:
	virtual void OnMatchStateSet() override;

};
