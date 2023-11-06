
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PEACK_API AWeapon : public AActor
{
	GENERATED_BODY()
	
/* FUNCTION */
public:	
	AWeapon();
	//virtual void Tick(float DeltaTime) override;

	void WeaponFire();

protected:
	virtual void BeginPlay() override;

/* PROPPERTY */
private:	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TObjectPtr<UAnimationAsset> AA_Fire;

};
