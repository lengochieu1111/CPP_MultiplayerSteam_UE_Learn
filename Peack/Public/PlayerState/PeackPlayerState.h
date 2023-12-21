
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PeackPlayerState.generated.h"

class APeackPlayerController;

UCLASS()
class PEACK_API APeackPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APeackPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void OnRep_Score()override;

	void AddOne_Score();
	void UpdateText_Score();

	void AddOne_Death();
	void UpdateText_Death();

	float GetDeath() const
	{
		return this->Dealth;
	}

	void SetDeath(const float NewDeath);

protected:
	virtual void BeginPlay()  override;

private:
	bool IsLocallyControlled() const;

	UFUNCTION()
	void OnRep_Ready();

	UFUNCTION()
	void OnRep_Death();

private:
	UPROPERTY()
	TObjectPtr<APeackPlayerController> PeackPlayerController;

	UPROPERTY(ReplicatedUsing = OnRep_Ready)
	bool bReady = false;

	UPROPERTY(ReplicatedUsing = OnRep_Death)
	float Dealth = 0;

};
