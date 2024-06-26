
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PeackGameMode.generated.h"

namespace MatchState
{
	extern PEACK_API const FName ShowResult;
}

UCLASS()
class PEACK_API APeackGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	APeackGameMode();

	void RequestRespawn(ACharacter* GivenCharacter, AController* GivenController);

	void HandleCharacterDead(AController* InstigatorController, AController* VictimController);

protected:
	virtual void BeginPlay() override;

	virtual void OnMatchStateSet() override;

private:
	double GetWorldTime() const;

private:
	double StartLevelTime = 0.0;

	UPROPERTY(EditDefaultsOnly, Category = "Countdown")
	double TotalTime_Warmup = 10.0;

	UPROPERTY(EditDefaultsOnly, Category = "Countdown")
	double TotalTime_Match = 20.0;

	UPROPERTY(EditDefaultsOnly, Category = "Countdown")
	double TotalTime_ShowResult = 10.0;

};
