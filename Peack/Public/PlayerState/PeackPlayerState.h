
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PeackPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PEACK_API APeackPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay()  override;

private:
	bool IsLocallyControlled() const;

	UFUNCTION()
	void OnRep_Ready();

private:
	UPROPERTY(ReplicatedUsing = OnRep_Ready)
	bool bReady = false;

	
};
