
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PeackAnimInstance.generated.h"

class APeackCharacter;

UCLASS()
class PEACK_API UPeackAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	float StrafingValue = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	double AO_Rifle = 0.0;

private:
	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	TObjectPtr<APeackCharacter> PeackCharacter;
	
};
