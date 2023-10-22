
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PeackCharacter.generated.h"

UCLASS()
class PEACK_API APeackCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region FUNCTION

#pragma endregion

public:
	APeackCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;


#pragma region PROPERTY

#pragma endregion

};
