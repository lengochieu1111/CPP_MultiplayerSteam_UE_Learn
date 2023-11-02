
#include "AnimInstance/PeackAnimInstance.h"

#include "Character/PeackCharacter.h"
#include "Kismet/KismetMathLibrary.h"

void UPeackAnimInstance::NativeInitializeAnimation()
{
	this->PeackCharacter = Cast<APeackCharacter>(GetOwningActor());
}

void UPeackAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (this->PeackCharacter == nullptr) return;

	this->Velocity = this->PeackCharacter->GetVelocity();

	// Ground Speed
	this->GroundSpeed = UKismetMathLibrary::VSizeXY(this->Velocity);

	/*
	* Strafing Value
	*/
	const FRotator& AimRotator = this->PeackCharacter->GetBaseAimRotation();
	FRotator MovementRotator = UKismetMathLibrary::MakeRotFromX(this->Velocity);
	this->StrafingValue = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotator, AimRotator).Yaw;

	this->AO_Rifle = AimRotator.Pitch;

	if (this->PeackCharacter->IsLocallyControlled() == false && this->AO_Rifle > 90.0)
	{
		FVector2D InRange = FVector2D(270.0, 360.0);
		FVector2D OutRange = FVector2D(-90.0, 0.0);

		this->AO_Rifle = FMath::GetMappedRangeValueClamped(InRange, OutRange, this->AO_Rifle);
	}

}
