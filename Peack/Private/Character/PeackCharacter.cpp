

#include "Character/PeackCharacter.h"

APeackCharacter::APeackCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Adjust Mesh Component
	GetMesh()->AddLocalOffset(FVector(0.0, 0.0, -88.0));
	GetMesh()->AddLocalRotation(FRotator(0.0, -90.0, 0.0));

}

void APeackCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void APeackCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

